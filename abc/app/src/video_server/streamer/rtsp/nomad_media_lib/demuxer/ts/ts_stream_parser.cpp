
#include "demuxer/ts/ts_stream_parser.h"

#include "base/debug.h"
#include "demuxer/ts/ts_pat.h"
#include "demuxer/ts/ts_pmt.h"
#include "demuxer/ts/es_parser.h"
#include "demuxer/ts/pes_parser.h"
#include "demuxer/ts/ts_index_generate.h"

#include "reader/file_reader.h"

#ifdef DUMP_TS
#include "sinker/file_sinker.h"
#endif
namespace nml_demuxer
{
namespace ts
{

double TsStreamParser::getDuration()
{
    double duration = indexFile.getPlayingDuration();
    return duration;
}

TsStreamParser::TsStreamParser() :

    pts(kNoTimestamp),
    firstPts(kNoTimestamp),
    previousPts(kNoTimestamp),  
    position(kNoTimestamp),
    scaleFactor(10),
    packetNumber(0),
    prevKeyFramePacketNumber(0),
    frameType(kDeriverFrame)
{

}

TsStreamParser::~TsStreamParser()
{
}

#ifdef DUMP_TS
nml_sinker::FileSinker sinker;
#endif

void TsStreamParser::init(const std::string& file_name)
{
    nml_reader::DataReaderParams reader_params;

    reader_params.buffer_size_ = 0;
    reader_params.file_name_ = file_name;

    Status stauts;
    if((stauts = fileReader.init( &reader_params ) )!= NML_OK)
        return;

    initIndexFile(file_name);
}

bool TsStreamParser::initIndexFile(const std::string& file_name)
{
    TS_LOG(LOG_LEVEL_TS_INDEX) << "[initIndexFile] file_name: " << file_name;
    Status result = indexFile.init(file_name);
    if (NML_ERROR_NOT_INITIALIZED == result)
    {
        TsIndexGenerate indexer;
        if (false == indexer.tsIndexer(file_name, false))
        {
            TS_LOG(LOG_LEVEL_ERROR) << red << "[initIndexFile Fails!!] " << reset;
            return false;
        }

        result = indexFile.init(file_name);
    }

	int64 first_pts = 0;
    //index loading to hash map
    uint32 index_count = indexFile.getNumberIndexRecords();
    indexFile.readIndexRecord(0);
    while (indexFile.getCurrentIndexRecordNum() <= index_count)
    {
        uint64 packet_num = indexFile.tsPacketNumFromBuffer();
        int64 pts = indexFile.ptsFromBuffer();

		if (first_pts == 0)
			first_pts = pts;

        float npt_time = (float)( (pts - first_pts) / 90000.0f);
		indexHash.insert( std::make_pair( packet_num, pts));
		indexBiHash.insert( std::make_pair( npt_time, packet_num));

        //FIXME: fix bug that don't read last record of index file
        if (false == indexFile.readCurrentIndexRecord() )
            break;
    }

#ifdef DUMP_TS
    nml_sinker::BaseSinkerParams sinker_params;
    sinker_params.buffer_size_ = 0;
    sinker_params.file_name_ = "./dump.ts";

    if ( (sinker.init( &sinker_params)) != NML_OK)
        return false;
#endif

    TS_LOG(-1) << green << "[initIndexFile Success!!] " << reset;
    return true;
}

void TsStreamParser::setScaleFactor(int trick_scale)
{
    frameType = kDeriverFrame;
    this->scaleFactor = trick_scale;
    TS_LOG(-1) << yellow << "@@@@!!  trick_scale:" << this->scaleFactor << reset;
}


Status TsStreamParser::parse(SendEvent e /* = nullptr */)
{
    size_t read_size = TS_PACKET_SIZE * TS_PACKETS_PER_NETWORK_PACKET;
    uint8 data[TS_PACKET_SIZE * TS_PACKETS_PER_NETWORK_PACKET];

    Status result = fileReader.getData(data, read_size);

    if (result != NML_ERROR_END_OF_STREAM)
        result = parse(data, read_size, e);

    if ( result == NML_ERROR_END_OF_STREAM ||
            result == NML_ERROR_BEGIN_OF_STREAM)
    {
        flush();
    }

    return result;
}

Status TsStreamParser::parse(const uint8* buffer, int size, SendEvent e)
{
    tsByteQueue.push(buffer, size);

    while (true)
    {
        const uint8* ts_buffer;
        int ts_buffer_size;
        tsByteQueue.peek(&ts_buffer, &ts_buffer_size);
        if (ts_buffer_size < TsPacket::kTsPacketSize)
        {
            //FIXME:
            size_t read_size = tsByteQueue.getBuffersize();

            if (nullptr != e &&  read_size != 0)
            {
                TS_LOG(1) << "$$$ read_size: " << read_size
                      << yellow <<"  packetNumber:" << packetNumber << reset;

                e(tsByteQueue.getBuffer(), tsByteQueue.getBuffersize());

                if (scaleFactor < 0 && prevKeyFramePacketNumber != 0
                        && frameType == kDropFrame)
                {
                    packetNumber = prevKeyFramePacketNumber - 1;
                    fileReader.movePosition( packetNumber * TS_PACKET_SIZE );

                    TS_LOG(1) << "$$$ position: " << fileReader.getPosition()
                                <<"  packetNumber:" << packetNumber;

                    prevKeyFramePacketNumber = 0;
                }

#ifdef DUMP_TS
                size_t buffer_size = tsByteQueue.getBuffersize();
                sinker.putData( tsByteQueue.getBuffer(), buffer_size);
#endif
            }

            break;
        }

        // Sync
        int skipped_bytes = TsPacket::sync(ts_buffer, ts_buffer_size);
        if (skipped_bytes > 0)
        {
            tsByteQueue.pop(skipped_bytes);
            continue;
        }

        // Parse the TS header
        packetNumber++;
        Status result = tsPacket.parse(ts_buffer, ts_buffer_size, packetNumber);
        if (NML_OK != result)
        {
            TS_LOG(LOG_LEVEL_ERROR) << "invalid TS packet";
            tsByteQueue.pop(1);
            continue;
        }

        // Parse the section.
        int pid = tsPacket.getPid();
        auto it = pidMaps.find(pid);

        if (it == pidMaps.end() && pid == TsSection::kPidPat)
        {
            TsSection* pat_parser = new TS_Pat(this);
            PidState pat_pid_state;
            pat_pid_state.init(pid, PidState::kPidPat, pat_parser);
            pat_pid_state.enable();

            it = pidMaps.insert( std::make_pair(pid, pat_pid_state)).first;
            //pat_parser->set_Pid_Maps(pid_maps_);
        }

        if (it == pidMaps.end() && pid == TsSection::kPidNullPacket)
        {
            PidState pat_pid_state;
            pat_pid_state.init(pid, PidState::kPidNullPacket, nullptr);
            pat_pid_state.enable();
            it = pidMaps.insert( std::make_pair(pid, pat_pid_state)).first;
        }
        if (it != pidMaps.end())
        {
#if 0
            //FIXME: pat, pmt 는 한번만 보낸다.
            if (pidMaps.size() > 3 )
            {
                if (it->second.getPidType() == PidState::kPidPat ||
                        it->second.getPidType() == PidState::kPidPmt)
                {
                    tsByteQueue.erase(TsPacket::kTsPacketSize);
                    //NM_SAFE_DELETE( ts_packet );
                    continue;
                }
            }
#endif
            if (scaleFactor != 10)
            {
                if (it->second.getPidType() == PidState::kPidAudioPes)
                {
                    tsByteQueue.erase(TsPacket::kTsPacketSize);
                    continue;
                }

                if ( (scaleFactor > 20 || scaleFactor < 0)
                    && it->second.getPidType() == PidState::kPidVideoPes)
                {
                    if (tsPacket.getPayloadUnitStartIndicator() == true)
                    {
                        auto index_iter = indexHash.find(packetNumber);

                        if ( index_iter == indexHash.end() )
                        {
                            frameType = kDropFrame;
                        }
                        else
                        {
                            frameType = kDeriverFrame;

                            if (scaleFactor < 0)
                            {
                                index_iter--;
                                if (index_iter != indexHash.begin())
                                    prevKeyFramePacketNumber =  index_iter->first;
                                else
                                {
                                    return NML_ERROR_BEGIN_OF_STREAM;
                                }

                                TS_LOG(1) << "$$$ prevKeyFramePacketNumber: " << prevKeyFramePacketNumber;
                            }
                        }
                    }

                    if ( frameType == kDropFrame )
                    {
                        TS_LOG(1) << "@@ X Frame Drop ts num:" << packetNumber;
                        tsByteQueue.erase(TsPacket::kTsPacketSize);
                        continue;
                    }
                    else
                    {
                        TS_LOG(1) << "@@ Deliver ts num:" << packetNumber;
                    }

                }
            }

            it->second.setTrickScale(scaleFactor);
            if ( NML_OK != it->second.pushTsPacket( tsPacket))
            {
                return NML_ERROR_INVALID_STREAM;
            }
        }
        else
        {
            tsByteQueue.erase(TsPacket::kTsPacketSize);
            continue;
            TS_LOG(-1) << "Ignoring TS packet for pid: " << tsPacket.getPid();
        }

        // Go to the next packet.
        tsByteQueue.pop(TsPacket::kTsPacketSize);

    } //while(true)

    return NML_OK;
}

Status TsStreamParser::trickModeFilter(int current_pid)
{

    return NML_OK;
}


Status TsStreamParser::seekStream(const float seek_npt_time)
{
    auto lower_iter = indexBiHash.lower_bound(seek_npt_time);
    uint64 packet_number = lower_iter->second;
    int64 pts = indexHash.find(packet_number)->second;
    //*rtp_time = pts;

    TS_LOG(-1) << green << " npt time:" << seek_npt_time << reset;

    if (NML_OK != fileReader.movePosition( (packet_number - 1) * TS_PACKET_SIZE ) )
    {
        TS_LOG(LOG_LEVEL_ERROR) << red << " !! seek movePosition Fail" << reset;
        tsByteQueue.reset();
        return NML_ERROR_FAILED;
    }

    packetNumber = packet_number - 1;

    TS_LOG(-1) << " ### current position :" <<  fileReader.getPosition()
               << " packetNumber: " << packetNumber;

    tsByteQueue.reset();
    //pts = static_cast<int64> (*rtp_time); //seek후에 처음 rtp_time이 seek전의 rtp_time을 보내는 버그떄문에 추가.
    tsPacket.reset(); //FIXME: tsPacket을 매번 새로 생성하는것에서 그렇지 않은것으로 변경후 sideeffect로 seek이 안되는 bug 수정.
    //pts = kNoTimestamp;
	 previousPts = kNoTimestamp;

    TS_LOG(LOG_LEVEL_INFO) << green << " !! seek movePosition Sucess" << reset;
    return NML_OK;
}

void TsStreamParser::flush()
{
    TS_LOG(LOG_LEVEL_INFO) << " TsStreamParser::flush ";
    packetNumber = 0;
    for ( auto it : pidMaps )
    {
        TS_LOG(LOG_LEVEL_INFO) << "Flushing and Delete PID" << it.first;
        PidState pid_state = it.second;
        pid_state.flush();
        //delete pid_state;
    }

    resetTimestamp();

    pidMaps.clear();
    tsByteQueue.reset();
}


void TsStreamParser::registerPmtCB(int program_number, int pmt_pid)
{
    TS_LOG (LOG_LEVEL_INFO) << green
                            << "program_number = " << program_number
                            << "pmt_pid = " << pmt_pid << reset;

    // Create the PMT state
    TsSection* pmt_parser (new TSPmt(this));
    PidState pmt_pid_state;
    pmt_pid_state.init(pmt_pid, PidState::kPidPmt, pmt_parser);
    pmt_pid_state.enable();

    pidMaps.insert( std::make_pair( pmt_pid, pmt_pid_state));
}



void TsStreamParser::registerPesCB(int pes_pid, int stream_type)
{
    TS_LOG (LOG_LEVEL_INFO) << green
                            << "pes_pid = " << pes_pid
                            << " stream type = " << stream_type << reset;

    auto it = pidMaps.find( pes_pid );
    if (it != pidMaps.end())
        return;

    bool is_audio = false;
    bool is_video = false;
    //ESParser* es_parser = nullptr;
    if ( stream_type == kStreamTypeAVC  ||
            stream_type == kStreamTypeMPEG2 ||
            stream_type == kStreamTypeMPEG4Video)
    {
        is_video = true;
        //es_parser = new EsAvcParser(this) ;
    }
    else if ( stream_type == kStreamTypeAAC || stream_type == kStreamTypeAC3
              || stream_type == kStreamTypeMPEG2Audio || stream_type == kStreamTypePes
              || stream_type == kStreamTypeMPEG1Audio)

    {
        //es_parser = new EsAdtsParser(this);
        is_audio = true;
    }
    else //is_private_section
    {
        return;
    }

    // Create the PES state
    if (is_video == false && is_audio == false)
    {
        return;
    }

    TsSection* pes_parser = new PESParser( CALLBACK_1(TsStreamParser::onVideoPts, this) );
    PidState::PidType pid_type = is_audio ? PidState::kPidAudioPes :
                                 PidState::kPidVideoPes;

    //PidState* pes_pid_state(
    //    new PidState( pes_pid, pid_type, pes_parser) );

    PidState pes_pid_state;
    pes_pid_state.init(pes_pid, pid_type, pes_parser);
    pes_pid_state.enable();

    pidMaps.insert( std::make_pair( pes_pid, pes_pid_state) );
}

 
void TsStreamParser::onVideoPts(const int64 packet_pts)
{
    TS_LOG(1) << yellow << "[onVideoPts]" << reset << " pts = "  << packet_pts;
    if (firstPts == kNoTimestamp)
        firstPts = packet_pts;

	if (previousPts == kNoTimestamp)
        previousPts = packet_pts;

    this->position = packet_pts - firstPts;

    if (scaleFactor < 0)
    {
        if (packet_pts < previousPts)
            this->pts = this->pts + ((previousPts - packet_pts)*10)/-scaleFactor;

    }
    else
    {
       if (packet_pts > previousPts)
       {
          this->pts = this->pts + ((packet_pts - previousPts)*10)/scaleFactor;
       }
    }

    previousPts = packet_pts;
}


} //namespace ts
} //namespace nml_demuxer




#include "ts_index_generate.h"

#include <chrono>
#include <iostream>
#include <string.h>

#include "base/debug.h"

#include "demuxer/ts/ts_stream_parser.h"
#include "demuxer/ts/pes_parser.h"
#include "demuxer/ts/es_avc_parser.h"

namespace nml_demuxer
{
namespace ts
{

TsIndexGenerate::~TsIndexGenerate()
{
}

bool TsIndexGenerate::tsIndexer(const std::string& input_file_name, bool perf_result_print)
{
    Status status;

    nml_reader::FileReader reader;
    nml_reader::DataReaderParams reader_params;

    reader_params.buffer_size_ = 0;
    reader_params.file_name_ = input_file_name;
    if ((status = reader.init( &reader_params ) ) != NML_OK)
        return false;

    nml_sinker::FileSinker sinker;
    nml_sinker::BaseSinkerParams sinker_params;

    std::string output_file_name(input_file_name + ".index");
    sinker_params.buffer_size_ = 0;
    sinker_params.file_name_ = output_file_name;

    if ( (status = sinker.init( &sinker_params)) != NML_OK)
        return false;

    BANCHMARK_START();
    indexerImpl(&reader, &sinker);
    BANCHMARK_END();

    if (perf_result_print == true)
        BANCHMARK_RESULT();

    if (NML_OK == status)
        return true;

    return false;
}

bool TsIndexGenerate::isH264KeyFrame(uint8* raw_es, int es_packet_size)
{
    int es_position = 0;
    int current_nal_position = -1;
    NalType nal_unit_type = kNoNalType;

    for ( ; es_position < es_packet_size - 4; es_position++)
    {
        // syncword  00 00 00 01 or 00 00 01
        if (raw_es[es_position + 0] != 0 || raw_es[es_position + 1] != 0)
            continue;

        int syncword_length = 0;
        if (raw_es[es_position + 2] == 0 && raw_es[es_position + 3] == 1)
            syncword_length = 4;
        else if (raw_es[es_position + 2] == 1)
            syncword_length = 3;
        else
            continue;

        // Parse the current NAL.
        if (current_nal_position >= 0)
        {
            int nal_size = es_position - current_nal_position;
            //NM_ASSERT(nal_size > 0);
            if (nal_size <= 0)
                return false;
        }
        current_nal_position = es_position + syncword_length;

        // Retrieve the NAL type.
        int nal_header = raw_es[current_nal_position];
        int forbidden_zero_bit = (nal_header >> 7) & 0x1;
        TS_CHECK(forbidden_zero_bit == 0);
        nal_unit_type = static_cast<NalType>(nal_header & 0x1f);

        TS_LOG(LOG_LEVEL_ES)
                << "nal: offset=" << es_position
                << " type=" << nal_unit_type;

        // Skip the syncword.
        es_position += syncword_length;
    }

    if (nal_unit_type == kIDRSlice)
        return true;

    if (nal_unit_type == kNonIDRSlice)
    {
        uint32 slice_type;
        uint32 first_mb_slice;
        BitReaderAVC bit_reader( raw_es + (current_nal_position + 1), es_packet_size - current_nal_position - 1);
        bit_reader.readBitsExpGolomb(&first_mb_slice);
        bit_reader.readBitsExpGolomb(&slice_type);

        if (2 == slice_type || 7 == slice_type )
            return true;

        return false;
    }

    return false;
}

bool TsIndexGenerate::isMPEG2IntraFrame(uint8* raw_es, int es_packet_size)
{
    int es_position = 0;

    for ( ; es_position < es_packet_size - 4; es_position++)
    {
#if 1 //FIXME: aspect_ratio¸¦ °¡Áö°í I FrameÆÇº°
        //00 00 01 B3 sequence header code
        int32 sequence_header_code =
            ((int32)raw_es[0]  << 24) |
            ((int32)raw_es[1] << 16) |
            ((int32)raw_es[2] << 8) |
            ((int32)raw_es[3]  );

        if (sequence_header_code == 0x000001b3)
        {
            // aspect_ratio  1 = 1:1 , 2 = 4:3 ,3 = 16:9
            // frame_rate  1 = 23.976 , 2 = 24 ,3 = 25, 4 = 29.97
            int aspect_ratio = (raw_es[7] & 0xF0) >> 4;
            //int frame_rate = (raw_es[7] & 0x0F);

            if (aspect_ratio == 2 || aspect_ratio == 3)
                return true;
        }
#endif

        // syncword  00 00 01 00 --> picture start code
        // 00 00 01 B8 --> start of group of pictures header
        if (raw_es[es_position + 0] != 0 || raw_es[es_position + 1] != 0)
            continue;

        int syncword_length = 0;
        if (raw_es[es_position + 2] == 1 && raw_es[es_position + 3] == 0)
            syncword_length = 4;
        else
            continue;

        //001 = I frame 010 = P frame 011 = B frame
        int frame_type = (raw_es[es_position + syncword_length + 1] & 0x38 ) >> 3;
        if (frame_type == 1)
            return true;


        TS_LOG(LOG_LEVEL_ES)
                << " picture start code: offset=" << es_position
                << " type=" << frame_type;

        // Skip the syncword.
        es_position += syncword_length;
    }

    return false;
}

bool TsIndexGenerate::isMPEG4IntraFrame(uint8* raw_es, int es_packet_size)
{
    int es_position = 0;

    for ( ; es_position < es_packet_size - 4; es_position++)
    {
        // syncword  00 00 01 b6 --> vop start code

        if (raw_es[es_position + 0] != 0 || raw_es[es_position + 1] != 0)
            continue;

        int syncword_length = 0;
        if (raw_es[es_position + 2] == 1 && raw_es[es_position + 3] == 0xb6)
            syncword_length = 4;
        else
            continue;

        //00  intra-coded (I),  01  predictive-coded (P)
        //10  bidirectionally-predictive-coded (B) ,11  sprite (S)
        int frame_type = (raw_es[es_position + syncword_length] & 0xb0 ) >> 6;
        if (frame_type == 0)
            return true;


        TS_LOG(LOG_LEVEL_ES)
                << " picture start code: offset=" << es_position
                << " type=" << frame_type;

        // Skip the syncword.
        es_position += syncword_length;
    }

    return false;
}


uint16 TsIndexGenerate::parsePMT(uint8* pkt, int size, uint8* video_stream_type)
{

    int section_length = ((pkt[2] & 0x0F) << 8) | pkt[3];
    if ( (4 + section_length) < size)
        size = (4 + section_length);

    // Then, skip any descriptors following the "program_info_length":
    if (size < 22)
        return 0; // not enough data

    int program_info_length = ((pkt[11] & 0x0F) << 8) | pkt[12];
    pkt += 13;
    size -= 13;
    if (size < program_info_length)
        return 0 ; // not enough data

    pkt += program_info_length;
    size -= program_info_length;

    // Look at each ("stream_type","elementary_PID") pair, looking for a video stream
    // ("stream_type" == 1 or 2):
    while (size >= 9)
    {
        uint8 stream_type = pkt[0];
        uint16 elementary_PID = ((pkt[1] & 0x1F) << 8) | pkt[2];
        if (stream_type == 1 || stream_type == 2 || stream_type == 0x10
                || stream_type == 0x1B/*H.264 video*/)
        {
            *video_stream_type = stream_type;
            return elementary_PID;
        }

        uint16 ES_info_length = ((pkt[3] & 0x0F) << 8) | pkt[4];
        pkt += 5;
        size -= 5;

        if (size < ES_info_length)
            return 0; // not enough data

        pkt += ES_info_length;
        size -= ES_info_length;
    }

    return 0;
}

void TsIndexGenerate::truncateFloatToInt( float value, uint8* index )
{
    int64 pcr_int = static_cast<int64>(value);
    index[4] = (uint8)(pcr_int & 0xFF);
    index[5] = (uint8)((pcr_int & 0xFF00) >> 8);
    index[6] = (uint8)((pcr_int & 0xFF0000) >> 16);
    uint8 pcr_fraction =
        (uint8)(( value - (float)(pcr_int)) * 255.0f);
    index[7] = pcr_fraction;
}


Status TsIndexGenerate::indexerImpl(nml_reader::DataReader* data_reader,
                                    nml_sinker::BaseSinker* data_sinker)
{
    uint32 packet_count = 0;


    uint8 buffer[TS_PACKET_SIZE];
    uint8* pes_raw_header;
    uint8* es_raw;
    uint8* timestamp_section;

    size_t read_size = TS_PACKET_SIZE;

    parserState = kHeader;

    uint32 video_pid;

    int64 first_timestamp = kNoTimestamp;
    int64 last_timestamp  = kNoTimestamp;

    uint64 key_frame_count = 0;
    uint8  video_stream_type = 0;

    while (NML_ERROR_END_OF_STREAM != data_reader->getData(buffer, read_size))
    {

        int pes_offset = 0;
        //int es_offset = 0;

        // General TS values
        uint8  sync_byte = buffer[0];
        uint8  transport_error_indicator = (buffer[1] & 0x80) >> 7;
        uint8  payload_unit_start_indicator = (buffer[1] & 0x40) >> 6;
        uint32 pid = (((uint32)buffer[1] & 0x1F) << 8) + (uint32)buffer[2];
        uint8  transport_scrambling_control = (buffer[3] & 0xC0) >> 6;
        uint8  adaptation_field_control = (buffer[3] & 0x30) >> 4;


        packet_count++;
        pes_offset += 4;

        if (adaptation_field_control == 2 || adaptation_field_control == 3)
        {
            uint8 adaptation_field_length = buffer[4];
            pes_offset += 1;
            pes_offset += adaptation_field_length;
        }

        // Check sync byte
        if (sync_byte != 0x47)
        {
            printf("ERROR: Missing the TS sync byte!\n");
            read_size = 0;
        }


        // Searching PMT
        if (parserState == kHeader)
        {
            if (transport_error_indicator == 0 &&
                    payload_unit_start_indicator == 1 &&
                    pid != 0x1FFF &&
                    transport_scrambling_control == 0 &&
                    adaptation_field_control == 1)
            {
                if (!(buffer[4] == 0 && buffer[5] == 0 && buffer[6] == 1))
                {
                    if (buffer[4] == 0 && buffer[5] == 2 &&
                            (buffer[6] & 0xC0) == 0x80)
                    {
                        printf("- PMT pid is %d (0x%X) Packet number-%d \n",
                               pid,pid ,
                               packet_count);

                        //video_pid = (( (uint32)buffer [13] & 0x1F) << 8) +
                        //    (uint32)buffer[14];
                        video_pid = parsePMT(&buffer[pes_offset], 188 - pes_offset,
                                             &video_stream_type);

                        printf("- PCR  pid is %d (0x%X) - video pid\n",video_pid,video_pid);

                        // Next, search the first PCR
                        parserState = kTsPayload;
                    }
                }
            }
        } //if (parserState == kHeader)


        bool is_key_frame_start = false;
        int pts_flag = 0;
        int dts_flag = 0;
        // Searching the PCR
        if (parserState == kTsPayload || parserState == kTsIndexing)
        {
            if (pid == video_pid && payload_unit_start_indicator == 1)
            {
                pes_raw_header = &buffer[pes_offset];

                uint8 pes_start_code_prefix =  (pes_raw_header[0] || pes_raw_header[1] || pes_raw_header[2]);
                if (pes_start_code_prefix == 0x000001)
                {
                    uint8 stream_id = pes_raw_header[3];
                    if (stream_id == 0xE0) //video pes
                    {
                        pts_flag = (pes_raw_header[7] & 0xC0) >> 7;
                        dts_flag = (pes_raw_header[7] & 0x40) >> 6;
                        uint8 pes_header_data_length = pes_raw_header[8];

                        int es_offset = 6 + 3 + pes_header_data_length;
                        es_raw = &pes_raw_header[es_offset];

                        int es_packet_size = 188 - (pes_offset + es_offset);

                        if (video_stream_type == 0x1b)
                            is_key_frame_start = isH264KeyFrame(es_raw, es_packet_size);
                        else if(video_stream_type == 0x1 ||
                                video_stream_type == 0x2 ||
                                video_stream_type == 0x80)
                        {
                            is_key_frame_start = isMPEG2IntraFrame(es_raw, es_packet_size);
                        }
                        else if (video_stream_type == 0x10)
                            is_key_frame_start = isMPEG4IntraFrame(es_raw, es_packet_size);

                    }
                }

                //timestamp
                int64 pts = kNoTimestamp;
                int64 dts = kNoTimestamp;
                if (pts_flag == 1)
                {
                    pts =
                        ((uint64)pes_raw_header[9]  << 32) |
                        ((uint64)pes_raw_header[10] << 24) |
                        ((uint64)pes_raw_header[11] << 16) |
                        ((uint64)pes_raw_header[12] << 8) |
                        ((uint64)pes_raw_header[13] );

                    pts = PESParser::convertTimestampSectionToTimestamp(pts);
                }


                if (dts_flag == 1)
                {
                    dts =
                        ((uint64)pes_raw_header[14]  << 32) |
                        ((uint64)pes_raw_header[15] << 24) |
                        ((uint64)pes_raw_header[16] << 16) |
                        ((uint64)pes_raw_header[17] << 8) |
                        ((uint64)pes_raw_header[18] );

                    dts = PESParser::convertTimestampSectionToTimestamp(dts);
                }

                if (dts < pts && dts != kNoTimestamp && dts != 0)
                {
                    if (first_timestamp < dts)
                        last_timestamp = dts;

                    timestamp_section = &pes_raw_header[14];
                }
                else
                {
                    if (first_timestamp < pts)
                        last_timestamp = pts;

                    timestamp_section = &pes_raw_header[9];
                }

                if (parserState == kTsPayload)
                {
                    //start kTsIndexing
                    parserState = kTsIndexing;
                    first_timestamp = last_timestamp;
                }
            } //if (pid == video_pid && payload_unit_start_indicator == 1)

        } //if (parserState == kTsPayload || parserState == kTsIndexing)


        //double Index_PCR = 0;
        if ( parserState == kTsIndexing && is_key_frame_start == true )
        {
            if (pid == video_pid &&
                    transport_error_indicator == 0 &&
                    payload_unit_start_indicator == 1)
            {
                key_frame_count++;

                //if (Index_PCR != last_pcr)
                {
                    uint8 index[9];

                    // TS packet number - Starting at 1
                    index[0] = (uint8)(packet_count & 0xFF);
                    index[1] = (uint8)((packet_count & 0xFF00) >> 8);
                    index[2] = (uint8)((packet_count & 0xFF0000) >> 16);
                    index[3] = (uint8)((packet_count & 0xFF000000) >> 24);

                    memcpy(index+4, timestamp_section, 5);

                    //double npt = static_cast<float>(last_timestamp / 90000.0f);
                    //printf("packet number:%d, pts_time:0.3f \n", packet_count, npt);

                    // std::cout << "packet number: " << yellow << packet_count
                    //         << "   npt_time:" << npt << reset << std::endl;

                    // Writing the index
                    size_t size = sizeof(index);
                    data_sinker->putData(index, size);
                }
            }
        }
    } //while

    // Show stats
    printf(" * %llu I Frame start packets were founded\n",key_frame_count);
#if 1
    if (last_timestamp > 0)
    {
        durationOfMovie = (float)(last_timestamp - first_timestamp) / 90000.0f;
#if 1
        std::cout << " * Movie duration is " << yellow << durationOfMovie
                  << " seconds" << reset << std::endl;
#endif
        uint8 duration_record[9];
        //write key_frame_countto index file
        duration_record[0] = (uint8)(key_frame_count & 0xFF);
        duration_record[1] = (uint8)((key_frame_count & 0xFF00) >> 8);
        duration_record[2] = (uint8)((key_frame_count & 0xFF0000) >> 16);
        duration_record[3] = (uint8)((key_frame_count & 0xFF000000) >> 24);

        //write duration to index file
        truncateFloatToInt(durationOfMovie, duration_record);
        size_t size = sizeof(duration_record);
        data_sinker->putData(duration_record, size);
    }
#endif
    return NML_OK;
}
} //namespace ts
} //namespace nml_demuxer



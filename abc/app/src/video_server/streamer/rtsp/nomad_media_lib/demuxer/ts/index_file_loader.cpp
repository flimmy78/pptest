
#include "index_file_loader.h"

#include <string.h>

#include "base/debug.h"
#include "demuxer/ts/pes_parser.h"

namespace nml_demuxer
{
namespace ts
{


IndexFileLoader::IndexFileLoader() :
    currentIndexRecordNum(-1),
    numberIndexRecords(0),
    frameCount(0),
	isKeyFrameStart(false)
{
    indexBuffer = new uint8[INDEX_RECORD_SIZE];
    memset(indexBuffer, 0x00, INDEX_RECORD_SIZE);
}

Status IndexFileLoader::init(const std::string& file_name)
{
    TS_LOG(LOG_LEVEL_TS_INDEX) << "[IndexFileLoader] init";
    std::string index_file_name = file_name +".index";
    reader_params.buffer_size_ = 0;
    reader_params.file_name_ = index_file_name;

    Status stauts;
    if((stauts = fileReader.init( &reader_params ) )!= NML_OK)
    {
        TS_LOG(LOG_LEVEL_TS_INDEX) << "index file doesn't exist!! ";
        return NML_ERROR_NOT_INITIALIZED;
    }

    uint64 index_file_size = fileReader.getSize();
    numberIndexRecords =  (index_file_size / INDEX_RECORD_SIZE) -1  ;
    TS_LOG(LOG_LEVEL_TS_INDEX) << " ### index_file_size :" << index_file_size
                               << " numberIndexRecords :" << numberIndexRecords;

    if (getPlayingDuration() == 0.0f)
    {
        return NML_ERROR_FAILED;
    }

    TS_LOG(LOG_LEVEL_TS_INDEX) << "[IndexFileLoader] init success!!";
    return NML_OK;
}

IndexFileLoader::~IndexFileLoader()
{
    NM_SAFE_DELETE_ARRAY(indexBuffer);
    TS_LOG(LOG_LEVEL_TS_INDEX) << "[IndexFileLoader] destroy";
}


void IndexFileLoader::lookupTSPacketNumFromNPT( float& npt, uint64& packet_number,
        uint64& index_number )
{
    if ( npt <= 0.0 || numberIndexRecords == 0 )
    {
        npt = 0.0f;
        packet_number = index_number = 0;
        return;
    }

    int64 seek_pts_time = static_cast<int64>(npt * 90000);
    uint64 found_index = 0;

    do
    {
        uint64 left_index = 0;
        uint64 right_index = numberIndexRecords;
        int64 left_pts = kNoTimestamp;
        int64 right_pts = kNoTimestamp;

        if ( !readIndexRecord( right_index ) )
            break;

        right_pts = ptsFromBuffer();
        TS_LOG(LOG_LEVEL_TS_INDEX) << " pcrRight" << right_pts;
        if ( seek_pts_time > right_pts )
            seek_pts_time = right_pts;


        while ( right_index - left_index > 1 && left_pts < seek_pts_time && seek_pts_time <= right_pts )
        {
            uint64 new_index = left_index + (uint64)( ( ( seek_pts_time - left_pts )
                               / (right_pts - left_pts) )
                               * ( right_index - left_index ) );

            if ( new_index == left_index || new_index == right_index )
            {
                new_index = ( left_index + right_index ) / 2;
            }

            if ( !readIndexRecord( new_index ) )
                break;

            int64 new_pts = ptsFromBuffer();

            if ( new_pts < seek_pts_time )
            {
                left_pts = new_pts;
                left_index = new_index;
            }
            else
            {
                right_pts = new_pts;
                right_index = new_index;
            }
        }

        if ( right_index - left_index > 1 || seek_pts_time <= left_pts || seek_pts_time > right_pts )
            break;

        found_index = right_index;
        //result = isStartFrame( found_index );
    }
    while ( 0 );


    if ( readIndexRecord( found_index ) )
    {
        int64 found_pts = ptsFromBuffer();
        npt = static_cast<float>(found_pts / 90000.0f);
        packet_number = tsPacketNumFromBuffer();
        index_number = found_index;

        TS_LOG(LOG_LEVEL_TS_INDEX) << " ### npt :" << npt
                                   << " tsPacketNumber :" << packet_number;
    }
    else
    {
        npt = 0.0f;
        packet_number = index_number = 0;
    }
}

void IndexFileLoader::lookupTSPacketNumFromRTPTime(uint32* rtp_time, uint64* ts_packet_number)
{

    if ( *rtp_time <= 0 || numberIndexRecords == 0 )
    {
        *rtp_time = 0;
        *ts_packet_number = 0;
        return;
    }

    int64 seek_pts_time = static_cast<int64>(*rtp_time);
    uint64 found_index = 0;

    do
    {
        uint64 left_index = 0;
        uint64 right_index = numberIndexRecords - 1;
        int64 left_pts = kNoTimestamp;
        int64 right_pts = kNoTimestamp;

        if ( !readIndexRecord( right_index ) )
            break;

        right_pts = ptsFromBuffer();
        //TS_LOG(LOG_LEVEL_TS_INDEX) << " pcrRight" << right_pts;
        if ( seek_pts_time > right_pts )
            seek_pts_time = right_pts;


        while ( right_index - left_index > 1 && left_pts < seek_pts_time && seek_pts_time <= right_pts )
        {
            uint64 new_index = left_index + (uint64)( ( ( seek_pts_time - left_pts )
                               / (right_pts - left_pts) )
                               * ( right_index - left_index ) );

            if ( new_index == left_index || new_index == right_index )
            {
                new_index = ( left_index + right_index ) / 2;
            }

            if ( !readIndexRecord( new_index ) )
                break;

            int64 new_pts = ptsFromBuffer();

            if ( new_pts < seek_pts_time )
            {
                left_pts = new_pts;
                left_index = new_index;
            }
            else
            {
                right_pts = new_pts;
                right_index = new_index;
            }
        }

        if ( right_index - left_index > 1 || seek_pts_time <= left_pts || seek_pts_time > right_pts )
            break;

        found_index = right_index;
        //result = isStartFrame( found_index );
    }
    while ( 0 );


    if ( readIndexRecord( found_index - 1) )
    {
        int64 found_pts = ptsFromBuffer();
        *rtp_time = static_cast<uint32>(found_pts);
        *ts_packet_number = tsPacketNumFromBuffer();
        TS_LOG(LOG_LEVEL_TS_INDEX) << " tsPacketNumber :" << *ts_packet_number;
    }
    else
    {
        *rtp_time = 0;
        *ts_packet_number = 0;
    }
}

/*
void IndexFileLoader::lookupPCRFromTSPacketNumber(
    const uint64 ts_packet_number,
    float& pcr,
    uint64& index_record_number )
{
    if ( ts_packet_number == 0 || numberIndexRecords == 0 )
    {
        pcr = 0.0f;
        index_record_number = 0;
        return;
    }

    uint64 ts_packet_num = ts_packet_number;

    bool result = false;
    uint64 found_index = 0;
    do
    {
        uint64 left_index = 0;
        uint64 right_index = numberIndexRecords - 1;
        uint64 left_ts_packet = 0;
        uint64 right_ts_packet = 0;

        if ( !readIndexRecord( right_index ) )
            break;

        right_ts_packet = tsPacketNumFromBuffer();

        if ( ts_packet_num > right_ts_packet )
            ts_packet_num = right_ts_packet;

        while ( right_index - left_index > 1
                && left_ts_packet < ts_packet_num
                && ts_packet_num <= right_ts_packet )
        {
            uint64 ixNew = left_index
                           + ( unsigned long )( ((ts_packet_number - left_ts_packet)
                                                 / ( right_ts_packet - left_ts_packet ) )
                                                * ( right_index - left_index ) );

            if ( ixNew == left_index || ixNew == right_index )
            {
                ixNew = ( left_index + right_index ) / 2;
            }

            if ( !readIndexRecord( ixNew ) )
                break;

            uint64 tsNew = tsPacketNumFromBuffer();

            if ( tsNew < ts_packet_num )
            {
                left_ts_packet = tsNew;
                left_index = ixNew;
            }
            else
            {
                right_ts_packet = tsNew;
                right_index = ixNew;
            }
        }

        if ( right_index - left_index > 1 ||
                ts_packet_num <= left_ts_packet ||
                ts_packet_num > right_ts_packet )
        {
            break;
        }

        found_index = right_index;
        //result = moveStartFrame( found_index );
    }
    while ( 0 );

    if ( result && readIndexRecord( found_index ) )
    {
        pcr = pcrFromBuffer();
        ts_packet_num = tsPacketNumFromBuffer();
        index_record_number = found_index;
    }
    else
    {
        pcr = 0.0f;
        index_record_number = 0;
    }

}
*/


float IndexFileLoader::getPlayingDuration()
{
    if ( numberIndexRecords == 0 || !readIndexRecord( numberIndexRecords - 1 ) )
        return 0.0f;

    if ( false == readIndexRecord(numberIndexRecords) )
        return 0.0f;
    unsigned pcr_int = ( indexBuffer[6] << 16 ) | ( indexBuffer[5] << 8 ) | indexBuffer[4];
    uint8 pcr_frac = indexBuffer[7];
    float duration = pcr_int + pcr_frac / 256.0f;
    return duration;
}

bool IndexFileLoader::seekToIndexRecord( uint64 index_record_number )
{

    if ( index_record_number == currentIndexRecordNum )
        return true;


    if (NML_OK != fileReader.movePosition(index_record_number * INDEX_RECORD_SIZE) )
        return false;

    currentIndexRecordNum = index_record_number;
    return true;
}


bool IndexFileLoader::readCurrentIndexRecord()
{
    return readIndexRecord(currentIndexRecordNum);
}

bool IndexFileLoader::readIndexRecord( uint64 index_record_number )
{
    do
    {
        if ( !seekToIndexRecord( index_record_number ) )
            break;

        size_t read_size = INDEX_RECORD_SIZE;

        if ( NML_OK != fileReader.getData(indexBuffer, read_size) )
            break;

        ++currentIndexRecordNum;
        return true;
    }
    while ( 0 );

    return false;
}

int64 IndexFileLoader::ptsFromBuffer()
{
    int64 pts =
        ((int64)indexBuffer[4]  << 32) |
        ((int64)indexBuffer[5] << 24) |
        ((int64)indexBuffer[6] << 16) |
        ((int64)indexBuffer[7] << 8) |
        ((int64)indexBuffer[8] );

    pts = PESParser::convertTimestampSectionToTimestamp(pts);
    return pts;
}

uint32 IndexFileLoader::tsPacketNumFromBuffer()
{
    return ( indexBuffer[3] << 24 ) | ( indexBuffer[2] << 16 ) | ( indexBuffer[1] << 8 ) | indexBuffer[0];
}

} //namespace ts
} //namespace nml_demuxer

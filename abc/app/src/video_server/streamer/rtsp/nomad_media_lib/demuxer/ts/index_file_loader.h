
#ifndef _INDEX_FILE_LOADER_H_
#define _INDEX_FILE_LOADER_H_

#include "reader/file_reader.h"
#include <string>

namespace nml_demuxer
{
namespace ts
{


#define INDEX_RECORD_SIZE 9

enum DropFrameType
{
    kDropFrame        = 0x00,
    kDeriverFrame     = 0x01,
};


class IndexFileLoader
{
public:
    IndexFileLoader();
    ~IndexFileLoader();

    Status init(const std::string& file_name);

    bool seekToIndexRecord( uint64 index_record_number );
    bool readIndexRecord( uint64 index_record_number );
    bool readCurrentIndexRecord();

    void lookupTSPacketNumFromNPT(float& npt, uint64& ts_packet_number,
                                  uint64& index_record_number);

    void lookupTSPacketNumFromRTPTime(uint32* rtp_time, uint64* ts_packet_number);


    float getPlayingDuration();
    //bool moveStartFrame(uint64& index_number );
    DropFrameType isDropFrame( uint64& index_number, int scale );

    uint32 tsPacketNumFromBuffer();
    //float  pcrFromBuffer();
    int64  ptsFromBuffer();

    uint64 getCurrentIndexRecordNum() const
    {
        return currentIndexRecordNum;
    }
    //void   setCurrentIndexRecordNum(uint64 index_num) { currentIndexRecordNum = index_num; }
    uint64 getNumberIndexRecords() const
    {
        return numberIndexRecords;
    }
private:
    nml_reader::FileReader fileReader;
    nml_reader::DataReaderParams reader_params;


    uint8  recordTypeFromBuffer()
    {
        return indexBuffer[0];
    }


    uint8* indexBuffer;
    uint64 currentIndexRecordNum;
    uint64 numberIndexRecords;
    int    frameCount;
	bool   isKeyFrameStart;
    NML_DECLARE_NON_COPYABLE(IndexFileLoader);
};

}
}

#endif //_INDEX_FILE_LOADER_H_




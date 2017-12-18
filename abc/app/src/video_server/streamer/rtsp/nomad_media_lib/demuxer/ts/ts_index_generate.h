
#ifndef _TS_INDEXER_H_
#define _TS_INDEXER_H_

#include <string>

#include "base/basic_types.h"
#include "base/nml_defines.h"
#include "base/nml_structure.h"

#include "reader/file_reader.h"
#include "sinker/file_sinker.h"

namespace nml_demuxer
{
namespace ts
{

//static char kVersion[] = "0.91";

class TsIndexGenerate
{
public:
    // ISO/IEC 14496 - Part 10(63page): Table 7-1 "NAL unit type codes"
    enum NalType
    {
        kNoNalType = 0,
        kNonIDRSlice = 1,
        kIDRSlice = 5,
        kSEI = 6, //Supplemental enhancement information
        kSPS = 7, //Sequence parameter set
        kPPS = 8, //Picture parameter set
        kAUD = 9, //access unit delimiter
    };


    enum ParserState
    {
        kHeader,
        kTsPayload,
        kTsIndexing,
    };

    TsIndexGenerate()
    {
        durationOfMovie = 0.0f;
        parserState = kHeader;
    }
    ~TsIndexGenerate();

    bool tsIndexer(const std::string& input_file_name, bool perf_result_print);

    float getDurationOfMovie() const
    {
        return durationOfMovie;
    }
private:

    Status indexerImpl(nml_reader::DataReader* data_reader,
                       nml_sinker::BaseSinker* data_sinker);

    bool isH264KeyFrame(uint8* raw_es, int es_packet_size);
    bool isMPEG2IntraFrame(uint8* raw_es, int es_packet_size);
    bool isMPEG4IntraFrame(uint8* raw_es, int es_packet_size);

    void truncateFloatToInt( float value, uint8* index );

    uint16 parsePMT(uint8* pkt, int size, uint8* video_stream_type);

    float durationOfMovie;
    ParserState parserState;
};

} //namespace ts
} //namespace nml_demuxer

#endif //_TS_INDEXER_H_





//=======================================================================
// 
//  [1/3/2014 kwanghyun won]
//                                                     
//======================================================================= 

#ifndef _ES_AVC_PARSER_H_
#define _ES_AVC_PARSER_H_

#include <list>

#include "base/nml_defines.h"
#include "base/media_time.h"
#include "base/byte_queue.h"
#include "base/debug.h"

#include "reader/bit_reader.h"
#include "demuxer/ts/es_parser.h"
namespace nml_demuxer
{
namespace ts
{

static const int kExtendedSar = 255;

// ISO 14496 part 10
// VUI parameters: Table E-1 "Meaning of sample aspect ratio indicator"
static const int kSarTableSize = 17;
static const int kTableSarWidth[kSarTableSize] = {
    0, 1, 12, 10, 16, 40, 24, 20, 32, 80, 18, 15, 64, 160, 4, 3, 2
};
static const int kTableSarHeight[kSarTableSize] = {
    0, 1, 11, 11, 11, 33, 11, 11, 11, 33, 11, 11, 33, 99, 3, 2, 1
};

// ISO/IEC 14496 - Part 10(63page): Table 7-1 "NAL unit type codes"
enum NalUnitType {
    kNalUnitTypeNonIDRSlice = 1,
    kNalUnitTypeIDRSlice = 5,
    kNalUnitTypeSEI = 6, //Supplemental enhancement information
    kNalUnitTypeSPS = 7, //Sequence parameter set
    kNalUnitTypePPS = 8, //Picture parameter set
    kNalUnitTypeAUD = 9, //access unit delimiter
};

class BitReaderAVC : public nml_reader::BitReader 
{
public:
    BitReaderAVC(const uint8* data, off_t size)
        : BitReader(data, size) { }

    // Read an unsigned exp-golomb value. (ue(v))
    bool readBitsExpGolomb(uint32* exp_golomb_value);
};

/*
unsigned exp-golomb code
1     ------> 0
010   ------> 1
011   ------> 2
00100 ------> 3
*/

inline bool BitReaderAVC::readBitsExpGolomb(uint32* exp_golomb_value)
{
    // 앞의 0의 갯수를 제거한다.
    int zero_count = 0;
    while (true) 
    {
        int one_bit;
        if (false == readBits(1, &one_bit) )
            return false;
        if (one_bit != 0)
            break;
        zero_count++;
    }

    // If zero_count is greater than 31, the calculated value will overflow.
    if (zero_count > 31)
    {
        skipBits(zero_count);
        return false;
    }

    // Read the actual value.
    uint32 base = (1 << zero_count) - 1;
    uint32 offset;
    if (false == readBits(zero_count, &offset) )
        return false;
    *exp_golomb_value = base + offset;

    return true;
}

class EsAvcParser : public ESParser
{
public:
    explicit EsAvcParser(EmitFrameInterface* cb);
    virtual ~EsAvcParser();

    //////////////////////////////////////////////////////////////////////////
    // ESParser implementation
    Status parse(const uint8* buffer, int size, 
                base::DeltaTime pts, 
                base::DeltaTime dts) OVERRIDE;

    void flush() OVERRIDE;
    void reset() OVERRIDE;

private:
    Status parseInternal();
    bool nalParser(const uint8* buffer, int size);
    Status processSPS(const uint8* buffer, int size);

    int convertToRbsp(const uint8* buffer, int size, uint8* buffer_rbsp);
    void startFrame(int audio_position);
    void flushFrame(int next_audio_position);
    void discardEs(int bytes);

    struct TimeStamp
    {
        base::DeltaTime dts;
        base::DeltaTime pts;
    };

    base::ByteQueue esByteQueue;
    std::list<std::pair<int, TimeStamp> > timeStampList;

    bool isKeyFrame;
    int  esPosition;
    int  currentNalPosition;
    int  currentAccessUnitPostition;

    EmitFrameInterface* emitFrameCB;
};


} //namespace ts
} //namespace nml_demuxer

#endif //_ES_AVC_PARSER_H_



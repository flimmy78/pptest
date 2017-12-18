#ifndef GW_NAL_UNIT_TOKENIZER_H
#define GW_NAL_UNIT_TOKENIZER_H

#include "_gwh264.h"
#include <list>
#include <stdint.h>

using namespace std;

//GW_H264_NAMESPACE_BEGIN

enum NAL_UNIT_TYPE
{
	eNUT_UNSPECIFIED = 0,
	eNUT_NON_IDR_PICTURE,
	eNUT_PARTITION_A,
	eNUT_PARTITION_B,
	eNUT_PARTITION_C,
	eNUT_IDR_PICTURE,
	eNUT_SEI,
	eNUT_SPS,
	eNUT_PPS,
	eNUT_AUD,
};

class NALUnitTokenizer
{
public:
	NALUnitTokenizer();
	NALUnitTokenizer(int nBufSize);
	~NALUnitTokenizer();

	int Init(uint8_t* pbBuffer, int nDataSize);
	int NextToken(
        uint8_t** ppBuffer, int* pUnitPayloadSize,
        int* pNALUnitType, bool bEndOfStream);
	int GetCount() const;

	int Input(uint8_t* pbBuffer, int nDataSize);
	int Reset();
	int Parse();
	int Clear();

	int GetDataSize() const;

private:
    struct NALUnit
    {
	    /**
	     * StartCodePrefix offset
	     */
	    int nPrefixOffset;

	    /**
	     * The beginning point where NALUnit's body exists.
	     */
	    int nOffset;
	    int nSize;
	    int nNALUnitType;
	    int nNALRefIndex;
    };

	int m_nPrefixOffset;
	int m_nDataSize;
	int m_nBufSize;
	uint8_t* m_pBuffer;

    list<NALUnit> m_NALUnitList;
};

//GW_H264_NAMESPACE_END

#endif //__NAL_UNIT_TOKENIZER_H__

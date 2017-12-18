#include "NALUnitTokenizer.h"
#include <nomad_log.h>
#ifdef WIN32
#include <crtdbg.h>
#endif

static const char* TAG = "H264";

//GW_H264_NAMESPACE_BEGIN

NALUnitTokenizer::NALUnitTokenizer() :
m_nBufSize(0),
m_pBuffer(nullptr),
m_nDataSize(0),
m_nPrefixOffset(0)
{
}

NALUnitTokenizer::~NALUnitTokenizer()
{
	if(m_nBufSize > 0)
	{
		delete[] m_pBuffer;
	}
}

NALUnitTokenizer::NALUnitTokenizer(int nBufSize) :
m_nBufSize(nBufSize),
m_nDataSize(0),
m_nPrefixOffset(0)
{
	m_pBuffer = new uint8_t [nBufSize];
}

#if 0
int NALUnitTokenizer::Parse()
{
	int nDataSize = m_nDataSize;

	int i;
	int nSize = nDataSize - 4;
	int nMax = sizeof(m_anOffset) / sizeof(m_anOffset[0]);
	m_nCount = 0;

	for(i=0; i<nSize; i++)
	{
		if( m_pBuffer[i  ] == 0 &&
			m_pBuffer[i+1] == 0 &&
			m_pBuffer[i+2] == 1    )
		{
			if(m_nCount == nMax)
			{
				ASSERT(false);
				return E_UNEXPECTED;
			}

			m_anOffset[m_nCount] = i;
			m_nCount++;
		}
	}

	return 0;
}
#else
int NALUnitTokenizer::Parse()
{
	const int nDataSize = m_nDataSize;

	int i;
	int nZeroCount = 0;
	NALUnit* pTailNALUnit;

    if ( m_NALUnitList.empty() )
    {
		pTailNALUnit = nullptr;
		i = 0;
    }
	else
	{
        pTailNALUnit = &m_NALUnitList.back();

		ASSERT(pTailNALUnit->nSize == 0);
		i = pTailNALUnit->nOffset;
	}

	for(; i<nDataSize; i++)
	{
		switch(m_pBuffer[i])
		{
		case 0x00:
			nZeroCount++;
			break;
		case 0x01:
			if(nZeroCount >= 2)
			{
				// StartCodePrefix
				if(pTailNALUnit)
				{
					pTailNALUnit->nSize = i - pTailNALUnit->nOffset - nZeroCount;
				}

                NALUnit NALUnit;
				NALUnit.nPrefixOffset = i - nZeroCount;
				NALUnit.nOffset = i + 1;
				NALUnit.nSize = 0;
				NALUnit.nNALUnitType = m_pBuffer[i+1] & 0x1F;
                m_NALUnitList.push_back(NALUnit);

                pTailNALUnit = &m_NALUnitList.back();

				LOGV(TAG, "%s(%d) %s: naltype(%d)", __CALL_INFO__, NALUnit.nNALUnitType);
			}
		default:
			nZeroCount = 0;
		}
	}

	return 0;
}
#endif

int NALUnitTokenizer::Init(uint8_t* pbBuffer, int nDataSize)
{
	m_NALUnitList.clear();
	m_pBuffer = pbBuffer;
	m_nDataSize = nDataSize;

	//int hr = Parse();

	return 0;
}

/**
 * Provide the next NALUnit to caller.
 *
 * @param ppBuffer
 * @param pUnitPayloadSize the size in bytes excluding start code prefix.
 * @param bEndOfStream Without waiting for the next nal unit, returns the last one.
 * @return
 */
int NALUnitTokenizer::NextToken(
	uint8_t** ppBuffer, int* pUnitPayloadSize, int* pNALUnitType, bool bEndOfStream)
{
	NALUnit NALUnit;
	int nSize;
	int hr;
	int nNALUnitType;

	if ( m_NALUnitList.empty() )
	{
		*ppBuffer = m_pBuffer;
		*pUnitPayloadSize = m_nDataSize;
		*pNALUnitType = eNUT_UNSPECIFIED;

		return -1;
	}

    NALUnit = m_NALUnitList.front();
    m_NALUnitList.pop_front();

	int nOffset = NALUnit.nOffset;
	// This variable is stored for Reset().
	m_nPrefixOffset = NALUnit.nPrefixOffset;
	nNALUnitType = NALUnit.nNALUnitType;

	// If this is the last nal unit.
    if ( m_NALUnitList.empty() )
	{
		if(bEndOfStream)
		{
			// The last NALUnit in this buffer.
			nSize = m_nDataSize - nOffset;
			hr = S_FALSE;
		}
		else
		{
			// We can't estimate this nal unit's size.
            m_NALUnitList.push_back(NALUnit);
			hr = -1;
		}
	}
	else
	{
		nSize = NALUnit.nSize;
		hr = 0;
	}

	if(hr == 0)
	{
		*ppBuffer = m_pBuffer + nOffset;
		*pUnitPayloadSize = nSize;
		*pNALUnitType = nNALUnitType;
	}

	return hr;
}

int NALUnitTokenizer::GetCount() const
{
	return (int) m_NALUnitList.size();
}

/**
 * Input additional data into buffer.
 * If buffer is not enough, reallocate it.
 * Parse() should be called after all data is ready.
 *
 * @param pbBuffer
 * @param nDataSize
 * @return
 */
int NALUnitTokenizer::Input(uint8_t* pbBuffer, int nDataSize)
{
	m_NALUnitList.clear();

	int nSize = m_nDataSize + nDataSize;

	// Buffer reallocation check
	if(nSize > m_nBufSize)
	{
		int nBufSize = m_nBufSize;
		uint8_t* pBuffer;

		while(true)
		{
			nBufSize <<= 1;
			if(nBufSize >= nSize)
			{
				break;
			}
		}

		pBuffer = new uint8_t [nBufSize];
		if(pBuffer == nullptr)
		{
			LOGE("ERROR", "%s(%d) %s: Failed to allocate memory.", __FILENAME__, __LINE__, __FUNCTION__); 
			return E_OUTOFMEMORY;
		}

		if(m_pBuffer)
		{
			memcpy(pBuffer, m_pBuffer, m_nDataSize);
			delete[] m_pBuffer;
		}

		LOGI(TAG, "%s(%d) %s: Reallocation happened. %d -> %d", __FILENAME__, __LINE__, __FUNCTION__, m_nBufSize, nBufSize);

		m_pBuffer = pBuffer;
		m_nBufSize = nBufSize;
	}

	memcpy(m_pBuffer + m_nDataSize, pbBuffer, nDataSize);
	m_nDataSize += nDataSize;

	ASSERT(m_nBufSize >= m_nDataSize);

	//int hr = Parse();
	return 0;
}

int NALUnitTokenizer::Reset()
{
	int nPrefixOffset;

    if( !m_NALUnitList.empty() )
	{
        NALUnit& NALUnit = m_NALUnitList.back();

		nPrefixOffset = NALUnit.nPrefixOffset;

		if(nPrefixOffset > 0)
		{
			m_nDataSize -= nPrefixOffset;
			memmove(m_pBuffer, m_pBuffer + nPrefixOffset, m_nDataSize);

			NALUnit.nPrefixOffset = 0;
			NALUnit.nOffset -= nPrefixOffset;
		}
	}
	else
	{
		// There is no NAL unit in the list.
		m_nDataSize = 0;

#if 1
	    ASSERT(GetCount() == 0);
#endif
	}

	return 0;
}

int NALUnitTokenizer::Clear()
{
	m_nDataSize = 0;
	m_nPrefixOffset = 0;
	m_NALUnitList.clear();

	return 0;
}

int NALUnitTokenizer::GetDataSize() const
{
	return m_nDataSize;
}

//GW_H264_NAMESPACE_END

#include "nomad.h"
#include "Flags.h"
#include "AACParser.h"
#include <math.h>
#include <string.h>
#include "BitBlock.h"
#include <stdio.h>
#include <nomad_log.h>

enum AAC_PARSER_STATE
{
	eAPS_INIT				= 0x00000000,
	eAPS_FRAME_START		= 0x00000001,
	eAPS_FRAME_END			= 0x00000002,
	eAPS_AAC_PROPERTIES		= 0x00000004,
};




enum AAC_FRAME_TYPE
{
	eAFT_ADTS				= 0,
	eAFT_RAW				= 1,
};

static const char* TAG = "AAC";

// CAACParser Implementation ============================================
CAACParser::CAACParser(int nBufSize) :
m_nBufSize(nBufSize),
m_nFrameType(eAFT_RAW)
{
	m_pFlags = new Flags();
	m_pbBuffer = new BYTE [nBufSize];
	Clear();
}

CAACParser::~CAACParser(void)
{
	delete[] m_pbBuffer;
	delete m_pFlags;
}

HRESULT CAACParser::Append(BYTE* pbBuffer, int nDataSize)
{
	int nIndex = m_nIndex + m_nDataSize;

#if 0
	static int _bFirst = 1;
	if(_bFirst)
	{
		FILE* fp = fopen("/sdcard/aac.out", "wb");
		fwrite(pbBuffer, 1, nDataSize, fp);
		fclose(fp);

		_bFirst = 0;
	}
#endif

    memcpy(m_pbBuffer + nIndex, pbBuffer, nDataSize);
    m_nDataSize += nDataSize;

	return S_OK;
}

HRESULT CAACParser::ArrangeData()
{
	if(m_nDataSize > 0)
	{
		memmove(m_pbBuffer, &m_pbBuffer[m_nIndex], m_nDataSize);
	}

	m_nIndex = 0;

	return S_OK;
}

HRESULT CAACParser::Input(BYTE* pbBuffer, int nDataSize)
{
	//LOGV_CALL(TAG, "[START]");

	HRESULT hr;

	if(nDataSize > m_nBufSize)
	{
		LOGE("ERROR", "Buffer is too small to store data. in CAACParser: bufsize(%d) datasize(%d)", m_nBufSize, nDataSize);

		return E_FAIL;
	}

	if(m_nDataSize + nDataSize > m_nBufSize)
	{
		LOGE("ERROR", "Out of memory in CAACParser().");
		Flush();
	}

    if(m_nIndex + m_nDataSize + nDataSize > m_nBufSize)
    {
		hr = ArrangeData();
    }

	hr = Append(pbBuffer, nDataSize);

	/*
	LOGV("AACParser", "CAACParser::Input() end index(%d) %d/%d",
		m_nIndex, m_nDataSize, m_nBufSize);
	*/
	//LOGV_CALL(TAG, "[END]");

	return S_OK;
}

/**
 * Get a Frame Data excluding ADTS Header.
 * @param ppBuffer
 * @param nDataSize
 */
HRESULT CAACParser::Output(BYTE** ppBuffer, int* pDataSize)
{
	//LOGV_CALL(TAG, "[START]");

	HRESULT hr;
	int nFrameLength;
	int nProtectionAbsent;


	if(m_nDataSize > 0 && m_nIndex > 0)
	{
		memmove(m_pbBuffer, &m_pbBuffer[m_nIndex], m_nDataSize);
		m_nIndex = 0;
	}




	if(m_nDataSize <= 6)
	{
		LOGV_CALL(TAG, "");
		LOGE(TAG, "%s(%d) %s:  m_nIndex(%d) m_nDataSize(%d) m_nBufSize(%d)", __CALL_INFO__, m_nIndex, m_nDataSize, m_nBufSize);
		return E_FAIL;
	}

#if 1
	hr = ParseAAC(
		&m_pbBuffer[m_nIndex], m_nDataSize, &nFrameLength, &nProtectionAbsent);
	//LOGV(TAG, "%s:%d ParseAAC() %X", __FILENAME__, __LINE__, hr);

	if(SUCCEEDED(hr))
	{
		const int nAUHeaderSize = 4;

		// m_nIndex indicates the starting point of AAC syncword.
		int nIndex = m_nIndex;
		int nFrameSize = nFrameLength;
		int nADTSHeaderSize;

		// FIXME: This code doesn't cover full spec of AAC ADTS.
		// Refer to 13818-7(AAC) P.21
		if(m_nFrameType == eAFT_RAW)
		{
			// AU Packet size including AV header.
			// If protection_absent is 0, ADTSHeader includes crc_check(16bits).
			nADTSHeaderSize = (nProtectionAbsent == 0) ? 9 : 7;

#if 0
			//This block handles AU Header.

			int nDiff = nADTSHeaderSize - nAUHeaderSize;
			nIndex += nDiff;
			nFrameSize -= nDiff;

			int nAACRawDataSize = (nFrameLength - nADTSHeaderSize) << 3;

			// See RFC 3640. AU Header (4 bytes)
			m_pbBuffer[nIndex]   = 0x00;
			m_pbBuffer[nIndex+1] = 0x10;

			// Little Endian -> Big Endian
			m_pbBuffer[nIndex+2] = (nAACRawDataSize & 0xFF00) >> 8;
			m_pbBuffer[nIndex+3] = nAACRawDataSize & 0x00FF;
#else
			// AACFrameLength excluding ADTSHeader (Unit: bytes)
			nFrameSize -= nADTSHeaderSize;
			nIndex += nADTSHeaderSize;
#endif
		}
		else
		{
			ASSERT(FALSE);
			LOGE(TAG, "%s(%d) %s:  m_nIndex(%d) m_nDataSize(%d) m_nBufSize(%d)", __CALL_INFO__, m_nIndex, m_nDataSize, m_nBufSize);
			return E_FAIL;
		}

		*ppBuffer = &m_pbBuffer[nIndex];
		*pDataSize = nFrameSize;

		m_nIndex += nFrameLength;
		m_nDataSize -= nFrameLength;

		/*
		LOGV("AACParser", "FrameLength(%d) Index(%d) DataSize(%d)",
			nFrameLength, m_nIndex, m_nDataSize);
		*/
	}
	else
	{
		LOGE(TAG, "%s(%d) %s:  m_nIndex(%d) m_nDataSize(%d) m_nBufSize(%d)", __CALL_INFO__, m_nIndex, m_nDataSize, m_nBufSize);
	}

	//LOGV_CALL(TAG, "[END]");

	return hr;
#else
	*ppBuffer = m_pbBuffer;
	*pDataSize = m_nDataSize;

	return S_OK;
#endif
}

HRESULT CAACParser::Output(BYTE** ppBuffer, int* pDataSize, int* pHeaderSize)
{
	//LOGV_CALL(TAG, "[START]");

	HRESULT hr;
	int nFrameLength;
	int nProtectionAbsent;

	if(m_nDataSize <= 6)
	{
		LOGV_CALL(TAG, "");
		return E_FAIL;
	}

#if 1
	hr = ParseAAC(
		&m_pbBuffer[m_nIndex], m_nDataSize, &nFrameLength, &nProtectionAbsent);
	//LOGV(TAG, "%s:%d ParseAAC() %X", __FILENAME__, __LINE__, hr);

	if(SUCCEEDED(hr))
	{
		const int nAUHeaderSize = 4;

		// m_nIndex indicates the starting point of AAC syncword.
		int nADTSHeaderSize;

		// FIXME: This code doesn't cover full spec of AAC ADTS.
		// Refer to 13818-7(AAC) P.21
		if(m_nFrameType == eAFT_RAW)
		{
			// AU Packet size including AV header.
			// If protection_absent is 0, ADTSHeader includes crc_check(16bits).
			nADTSHeaderSize = (nProtectionAbsent == 0) ? 9 : 7;

#if 0
			//This block handles AU Header.
			int nDiff = nADTSHeaderSize - nAUHeaderSize;
			nIndex += nDiff;
			nFrameSize -= nDiff;

			int nAACRawDataSize = (nFrameLength - nADTSHeaderSize) << 3;

			// See RFC 3640. AU Header (4 bytes)
			m_pbBuffer[nIndex]   = 0x00;
			m_pbBuffer[nIndex+1] = 0x10;

			// Little Endian -> Big Endian
			m_pbBuffer[nIndex+2] = (nAACRawDataSize & 0xFF00) >> 8;
			m_pbBuffer[nIndex+3] = nAACRawDataSize & 0x00FF;
#else
			// AACFrameLength excluding ADTSHeader (Unit: bytes)
			//nFrameSize -= nADTSHeaderSize;
			//nIndex += nADTSHeaderSize;
#endif
		}
		else
		{
			ASSERT(FALSE);
			return E_FAIL;
		}

		*ppBuffer = &m_pbBuffer[m_nIndex];
		*pDataSize = nFrameLength;
		*pHeaderSize = nADTSHeaderSize;

		m_nIndex += nFrameLength;
		m_nDataSize -= nFrameLength;

		/*
		LOGV("AACParser", "FrameLength(%d) Index(%d) DataSize(%d)",
			nFrameLength, m_nIndex, m_nDataSize);
		*/
	}

	//LOGV_CALL(TAG, "[END]");

	return hr;
#else
	*ppBuffer = m_pbBuffer;
	*pDataSize = m_nDataSize;

	return S_OK;
#endif
}

HRESULT CAACParser::Clear()
{
	Flush();
	m_pFlags->setFlags(eAPS_INIT);

	return S_OK;
}

HRESULT CAACParser::Flush()
{
	m_nIndex = 0;
	m_nDataSize = 0;

	//LOGI("AACParser", "[AACParser] Flushed.");

	return S_OK;
}

/**
 * Parse an AACFrame.
 */
HRESULT CAACParser::ParseAAC(
	BYTE* pbBuffer, int nDataSize, int* pFrameLength, int* pProtectionAbsent)
{
	const int anFrequency[] =
	{
		96000, 88200, 64000, 48000, 44100, 32000,
		24000, 22050, 16000, 12000, 11025, 8000
	};
	const int anFreqCount = sizeof(anFrequency) / sizeof(int);

	HRESULT hr;

	int i;
	int nSize = nDataSize - 7;

	// adts_fixed_header()
	UINT nSyncword;
	UINT nID;
	UINT nLayer;
	UINT nProtectionAbsent;
	UINT nProfile;
	UINT nSamplingFrequencyIndex;
	UINT nPrivateBit;
	UINT nChannelConfiguration;
	UINT nOriginalCopy;
	UINT nHome;

	// adts_variable_header()
	UINT nCopyrightIdentificationBit;
	UINT nCopyrightIdentificationStart;
	UINT nFrameLength;
	UINT nAdtsBufferFullness;
	UINT nNumberOfRawDataBlocksInFrame;

	for(i=0; i<nSize; i++)
	{
		//if( pbBuffer[i] == 0xFFu		&&
		//	pbBuffer[i+1] >= 0xF0u		&&
		//	pbBuffer[i+1] != 0xFFu		)
		if( pbBuffer[i] == 0xFFu && ((pbBuffer[i+1] & 0xF6) == 0xF0) )
		{
			// < Fixed Header of ADTS >
			// ID = 1 (1bit)
			// layer = 00 (2bit)
			// According to protection_absent's value
			// ADTSHeader size is changed. (0: 9bytes, 1:7bytes)
			break;
		}
	}

	m_nIndex += i;
	m_nDataSize -= i;
	LOGV(TAG, "%s(%d) %s: index(%d) size(%d)", __CALL_INFO__, i, m_nDataSize);

	// Failed to find a syncword in AACFrame.
	if(i == nSize)
	{
		LOGE(TAG, "%s(%d) %s:  m_nIndex(%d) m_nDataSize(%d) m_nBufSize(%d)", __CALL_INFO__, m_nIndex, m_nDataSize, m_nBufSize);
		return E_FAIL;
	}

	CBitBlock bitBlock;
	hr = bitBlock.SetBuffer(&pbBuffer[i]);

	// Fixed Header of ADTS -----------------------------
	// syncword bslbf(12)
	hr = bitBlock.GetValue(12, &nSyncword);
	// ID bslbf(1)
	hr = bitBlock.GetValue(1, &nID);
	// layer uimsbf(2)
	hr = bitBlock.GetValue(2, &nLayer);
	// protection_absent bslbf(1)
	hr = bitBlock.GetValue(1, &nProtectionAbsent);
	// profile uimsbf(2)
	hr = bitBlock.GetValue(2, &nProfile);
	// sampling_frequency_index uimsbf(4)
	hr = bitBlock.GetValue(4, &nSamplingFrequencyIndex);
	// private_bit bslbf(1)
	hr = bitBlock.GetValue(1, &nPrivateBit);
	// channel_configuration uimsbf(3)
	hr = bitBlock.GetValue(3, &nChannelConfiguration);
	// original/copy bslbf(1)
	hr = bitBlock.GetValue(1, &nOriginalCopy);
	// home bslbf(1)
	hr = bitBlock.GetValue(1, &nHome);

	// Variable Header of ADTS ------------------------------------------
	// copyright_identification_bit bslbf(1)
	hr = bitBlock.GetValue(1, &nCopyrightIdentificationBit);
	// copyright_identification_start bslbf(1)
	hr = bitBlock.GetValue(1, &nCopyrightIdentificationStart);
	// frame_length bslbf(13)
	hr = bitBlock.GetValue(13, &nFrameLength);
	// adts_buffer_fullness bslbf(11)
	hr = bitBlock.GetValue(11, &nAdtsBufferFullness);
	// number_of_raw_data_blocks_in_frame uimsfb(2)
	hr = bitBlock.GetValue(2, &nNumberOfRawDataBlocksInFrame); // 56bits

	/*
	LOGI("AACParser", "Index(%d) DataSize(%d) Profile(%d) sampleRateIndex(%d) channels(%d) FrameLength(%d)",
		m_nIndex, m_nDataSize, nProfile,
		nSamplingFrequencyIndex, nChannelConfiguration, nFrameLength);
	*/

	if(nSyncword != 0xfff)
	{
		LOGE(TAG, "%s(%d) %s:  m_nIndex(%d) m_nDataSize(%d) m_nBufSize(%d)", __CALL_INFO__, m_nIndex, m_nDataSize, m_nBufSize);
		return E_FAIL;
	}

	if( !m_pFlags->getFlags(eAPS_AAC_PROPERTIES) )
	{
		m_nProfile = nProfile;

		// SampleRate ------------------------------------
		if(nSamplingFrequencyIndex > 12)
		{
			LOGE("ERROR", "[ERROR] SamplingFrequencyIndex: %d", nSamplingFrequencyIndex);
			nSamplingFrequencyIndex = 3;
		}
		m_nSampleRate = anFrequency[nSamplingFrequencyIndex];

		// Channels --------------------------------------
		m_nChannels = nChannelConfiguration;
		if(m_nChannels <= 0)
		{
			LOGE("ERROR", "[ERROR] Channels: %d", m_nChannels);
			m_nChannels = 2;
		}

		m_pFlags->setFlags(eAPS_AAC_PROPERTIES, TRUE);
	}
#if 0
	else
	{
		if( m_nProfile != nProfile									||
			m_nSampleRate != anFrequency[nSamplingFrequencyIndex]	||
			m_nChannels != nChannelConfiguration )
		{
			m_nIndex += 2;
			m_nDataSize -= 2;
			return E_UNEXPECTED;
		}
	}
#endif

	if(nFrameLength > (unsigned int) m_nDataSize)
	{
		LOGE(TAG, "%s(%d) %s:  m_nIndex(%d) m_nDataSize(%d) m_nBufSize(%d) nFrameLength(%d)", __CALL_INFO__, m_nIndex, m_nDataSize, m_nBufSize, nFrameLength);
		return E_FAIL;
	}

	*pFrameLength = nFrameLength;
	*pProtectionAbsent = nProtectionAbsent;

	return S_OK;
}

HRESULT CAACParser::Skip(int nSize)
{
	if(nSize < 0)
	{
		return E_FAIL;
	}

	m_nIndex = 0;
	m_nDataSize -= nSize;

	if(m_nDataSize > 0)
	{
		memmove(m_pbBuffer, m_pbBuffer + nSize, m_nDataSize);
	}
	else
	{
		m_nDataSize = 0;
	}

	return S_OK;
}

HRESULT CAACParser::GetBuffer(BYTE** ppBuffer)
{
	*ppBuffer = m_pbBuffer;
	return S_OK;
}

int CAACParser::GetDataSize() const
{
	return m_nDataSize;
}

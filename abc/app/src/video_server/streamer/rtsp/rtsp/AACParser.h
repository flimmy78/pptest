#ifndef __AACPARSER_H__
#define __AACPARSER_H__

#include "AudioParser.h"

class Flags;
//class CBitBlock;
//class CCircularQueue;

/**
 * Don't consider AU Header.
 * Just Divide data into AAC frames with ADTS Header.
 */
class CAACParser : public CAudioParser
{
protected:
	Flags* m_pFlags;

	int m_nIndex;
	int m_nBufSize;
	int m_nDataSize;
	BYTE* m_pbBuffer;

	// AAC Data properties
	int m_nProfile;
	int m_nSampleRate;
	int m_nChannels;

	// The type of frame that aac parser provides.
	int m_nFrameType;

protected:
	HRESULT Flush();
	HRESULT Append(BYTE* pbBuffer, int nDataSize);
	HRESULT ArrangeData();
	HRESULT ParseAAC(
		BYTE* pbBuffer, int nDataSize,
		int* pFrameLength, int* pProtectionAbsent);

public:
	CAACParser(int nBufSize);
	virtual ~CAACParser(void);

	HRESULT Input(BYTE* pbBuffer, int nDataSize);
	HRESULT Output(BYTE** ppBuffer, int* pDataSize);
	HRESULT Output(BYTE** ppBuffer, int* pDataSize, int* pHeaderSize);
	HRESULT Clear();

	HRESULT GetBuffer(BYTE** ppBuffer);
	HRESULT Skip(int nSize);

	int GetDataSize() const;
};

#endif

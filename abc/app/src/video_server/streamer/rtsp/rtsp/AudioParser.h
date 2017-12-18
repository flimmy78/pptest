#ifndef __AUDIO_PARSER_H__
#define __AUDIO_PARSER_H__

class CAudioParser
{
public:
	virtual ~CAudioParser(void) {};

	virtual HRESULT Input(BYTE* pbBuffer, int nDataSize) = 0;
	virtual HRESULT Output(BYTE** ppBuffer, int* pDataSize) = 0;
	virtual HRESULT Clear() = 0;
};

#endif

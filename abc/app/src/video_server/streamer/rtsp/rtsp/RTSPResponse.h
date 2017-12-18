#ifndef __RTSP_RESPONSE_H__
#define __RTSP_RESPONSE_H__

#include "RTSPMessage.h"

class RTSPResponse : public RTSPMessage
{
protected:
	int statusCode;

public:
	RTSPResponse(void);
	virtual ~RTSPResponse(void);

	int getStatusCode();
	HRESULT setStatusCode(int nStatusCode);

	HRESULT getStatusLine(char* szBuf, int nBufSize);
};

#endif	// __RTSP_RESPONSE_H__
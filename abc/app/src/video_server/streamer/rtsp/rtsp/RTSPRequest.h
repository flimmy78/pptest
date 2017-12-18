#ifndef __RTSP_REQUEST_H__
#define __RTSP_REQUEST_H__

#include "RTSPMessage.h"

class RTSPRequest : public RTSPMessage
{
protected:
	int method;

	// RTSP URI
	int port;
	char* host;
	char* absolutePath;

protected:
	void releaseText(char** pszText);

public:
	RTSPRequest(void);
	virtual ~RTSPRequest(void);

	HRESULT getMethod(int* pMethod);
	HRESULT getMethod(char** pszMethod);
	HRESULT setMethod(int nMethod);

	HRESULT getRequestLine(char* szLine, int nBufSize);

	char* getHost();
	HRESULT setHost(char* szHost);

	char* getAbsolutePath();
	HRESULT setAbsolutePath(char* szPath);

	HRESULT setURL(const char* szURL);

	int getPort();
	HRESULT setPort(int nPort);

	HRESULT toString(char* pBuf, int nBufSize, int* pDataSize);
};

#endif	// __RTSP_REQUEST_H__
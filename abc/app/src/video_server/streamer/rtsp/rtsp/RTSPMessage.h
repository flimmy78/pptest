#ifndef __RTSP_MESSAGE_H__
#define __RTSP_MESSAGE_H__

class Header;
class LinkedList;

class RTSPMessage
{
protected:
	int majorVersion;
	int minorVersion;

	LinkedList* headerList;

	int contentLength;
	char* content;

protected:
	void releaseHeaders();
	void releaseContent();

public:
	RTSPMessage(void);
	virtual ~RTSPMessage(void);

	HRESULT getVersion(int* pMajorVersion, int* pMinorVersion);
	HRESULT setVersion(int nMajorVersion, int nMinorVersion);

	int getHeaderCount() const;
	//HRESULT AddHeader(int nHeader, const char* szValue);
	HRESULT addHeader(Header* pHeader);
	HRESULT getHeader(int nHeader, Header** ppHeader);
	HRESULT removeHeader(int nHeader, Header** ppHeader);

	int getContentLength();
	HRESULT setContentLength(int nContentLength);
	HRESULT getContent(char** ppContent, int* pContentLength);
	HRESULT setContent(const char* pContent, int nLength);

	HRESULT getHeaders(char* szBuf, int nBufSize);

	virtual HRESULT clear();
};

#endif	// __RTSP_MESSAGE_H__
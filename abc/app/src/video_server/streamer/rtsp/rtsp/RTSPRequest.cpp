#include <string.h>

#include "nomad.h"
#include "RTSPRequest.h"
#include "rtsp.h"
//#include "httpcore/httpcore.h"

/** @brief RTSPRequest 생성자. rtsp request message의 형태를 제공한다.
	@return 의미 없음.
	@param 없음.
*/
RTSPRequest::RTSPRequest(void) :
method(eRM_UNKNOWN),
port(-1),
host(NULL),
absolutePath(NULL)
{
}

/** @brief RTSPRequest 소멸자. rtsp request message의 형태를 제공한다.
	@return 의미 없음.
	@param 없음.
*/
RTSPRequest::~RTSPRequest(void)
{
	releaseString(&host);
	releaseString(&absolutePath);
}

/** @brief RTSPRequest method를 가져온다.
	@return S_OK
	@param pMethod 가져온 method의 index
*/
HRESULT RTSPRequest::getMethod(int* pMethod)
{
	*pMethod = method;
	return S_OK;
}

/** @brief RTSPRequest method를 설정한다..
	@return S_OK
	@param nMethod 설정할 method의 index
*/
HRESULT RTSPRequest::setMethod(int nMethod)
{
	method = nMethod;
	return S_OK;
}

/** @brief RTSPRequest host를 가져온다.
	@return 가져온 host
	@param 없음.
*/
char* RTSPRequest::getHost()
{
	return host;
}

/** @brief RTSPRequest host를 설정한다.
	@return S_OK
	@param szHost 설정할 host
*/
HRESULT RTSPRequest::setHost(char* szHost)
{
	//HRESULT hr;

	::releaseString(&host);
	::copyStringWithAllocation(szHost, &host);

	return S_OK;
}

/** @brief RTSPRequest url을 가져온다.
	@return rtsp url
	@param 없음.
*/
char* RTSPRequest::getAbsolutePath()
{
	return absolutePath;
}

/** @brief RTSPRequest url을 설정한다.
	@return S_OK
	@param szPath 설정할 url
*/
HRESULT RTSPRequest::setAbsolutePath(char* szPath)
{
	//HRESULT hr;

	::releaseString(&absolutePath);
	::copyStringWithAllocation(szPath, &absolutePath);

	return S_OK;
}

/** @brief 설정된 url을 파싱하여 host, userid, port 등을 셋팅한다.
	@return S_OK or E_FAIL
	@param szURL 파싱할 url
*/
HRESULT RTSPRequest::setURL(const char* szURL)
{
	HRESULT hr;
	char szProtocol[8];
	char szUserID[128];
	char szHost[128];
	char szAbsolutePath[128];
	int nPort;


	hr = ::parseURL(szURL, szProtocol, szUserID, szHost, &nPort, szAbsolutePath);
	if(FAILED(hr))
	{
		return hr;
	}

	hr = setHost(szHost);
	hr = setPort(nPort);
	hr = setAbsolutePath(szAbsolutePath);

	return hr;
}

/** @brief 설정된 port를 가져온다.
	@return port number
	@param 없음.
*/
int RTSPRequest::getPort()
{
	return port;
}

/** @brief port를 설정한다.
	@return S_OK
	@param nPort 설정할 포트
*/
HRESULT RTSPRequest::setPort(int nPort)
{
	port = nPort;
	return S_OK;
}

/** @brief 설정된 method, host, port등을 가지고 url string을 만든다.
	@return S_OK
	@param szBuf url을 만들 버퍼
	@param nBufSize url을 만들 버퍼 사이즈
*/
HRESULT RTSPRequest::getRequestLine(char* szBuf, int nBufSize)
{
	if(method == eRM_UNKNOWN)
	{
		return E_FAIL;
	}

	const char* szMethod = rtspGetMethodName(method);

	sprintf(szBuf, "%s rtsp://%s:%d%s RTSP/%d.%d\r\n",
		szMethod, host, port, absolutePath,
		majorVersion, minorVersion
	);

	return S_OK;
}

/** @brief 설정된 header와 body를 string 형태로 만들어 준다.
	@return S_OK
	@param pBuf string을 채울 버퍼
	@param nBufSize string을 채울 버퍼 크기
	@param pDataSize 실제 string의 크기
*/
HRESULT RTSPRequest::toString(char* pBuf, int nBufSize, int* pDataSize)
{
	HRESULT hr;
	int nLength;
	int nDataSize;
	char* szHeaders;
	char* pContent;

	// REQUEST LINE
	hr = getRequestLine(pBuf, nBufSize);
	nLength = strlen(pBuf);

	// HEADERS
	szHeaders = pBuf + nLength;
	nBufSize -= nLength;
	nDataSize = nLength;
	hr = getHeaders(szHeaders, nBufSize);
	nLength = strlen(szHeaders);
	nDataSize += nLength;

	// CONTENT
	hr = getContent(&pContent, &nLength);
	if(SUCCEEDED(hr) && nLength > 0)
	{
		memcpy(pBuf + nDataSize, pContent, nLength);
		nDataSize += nLength;
		pBuf[nDataSize] = '\0';
	}

	*pDataSize = nDataSize;

	return S_OK;
}

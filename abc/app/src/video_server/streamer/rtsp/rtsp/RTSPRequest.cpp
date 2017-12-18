#include <string.h>

#include "nomad.h"
#include "RTSPRequest.h"
#include "rtsp.h"
//#include "httpcore/httpcore.h"

/** @brief RTSPRequest ������. rtsp request message�� ���¸� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPRequest::RTSPRequest(void) :
method(eRM_UNKNOWN),
port(-1),
host(NULL),
absolutePath(NULL)
{
}

/** @brief RTSPRequest �Ҹ���. rtsp request message�� ���¸� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPRequest::~RTSPRequest(void)
{
	releaseString(&host);
	releaseString(&absolutePath);
}

/** @brief RTSPRequest method�� �����´�.
	@return S_OK
	@param pMethod ������ method�� index
*/
HRESULT RTSPRequest::getMethod(int* pMethod)
{
	*pMethod = method;
	return S_OK;
}

/** @brief RTSPRequest method�� �����Ѵ�..
	@return S_OK
	@param nMethod ������ method�� index
*/
HRESULT RTSPRequest::setMethod(int nMethod)
{
	method = nMethod;
	return S_OK;
}

/** @brief RTSPRequest host�� �����´�.
	@return ������ host
	@param ����.
*/
char* RTSPRequest::getHost()
{
	return host;
}

/** @brief RTSPRequest host�� �����Ѵ�.
	@return S_OK
	@param szHost ������ host
*/
HRESULT RTSPRequest::setHost(char* szHost)
{
	//HRESULT hr;

	::releaseString(&host);
	::copyStringWithAllocation(szHost, &host);

	return S_OK;
}

/** @brief RTSPRequest url�� �����´�.
	@return rtsp url
	@param ����.
*/
char* RTSPRequest::getAbsolutePath()
{
	return absolutePath;
}

/** @brief RTSPRequest url�� �����Ѵ�.
	@return S_OK
	@param szPath ������ url
*/
HRESULT RTSPRequest::setAbsolutePath(char* szPath)
{
	//HRESULT hr;

	::releaseString(&absolutePath);
	::copyStringWithAllocation(szPath, &absolutePath);

	return S_OK;
}

/** @brief ������ url�� �Ľ��Ͽ� host, userid, port ���� �����Ѵ�.
	@return S_OK or E_FAIL
	@param szURL �Ľ��� url
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

/** @brief ������ port�� �����´�.
	@return port number
	@param ����.
*/
int RTSPRequest::getPort()
{
	return port;
}

/** @brief port�� �����Ѵ�.
	@return S_OK
	@param nPort ������ ��Ʈ
*/
HRESULT RTSPRequest::setPort(int nPort)
{
	port = nPort;
	return S_OK;
}

/** @brief ������ method, host, port���� ������ url string�� �����.
	@return S_OK
	@param szBuf url�� ���� ����
	@param nBufSize url�� ���� ���� ������
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

/** @brief ������ header�� body�� string ���·� ����� �ش�.
	@return S_OK
	@param pBuf string�� ä�� ����
	@param nBufSize string�� ä�� ���� ũ��
	@param pDataSize ���� string�� ũ��
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

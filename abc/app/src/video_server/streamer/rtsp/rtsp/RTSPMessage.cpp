#include "nomad.h"
#include "rtsp.h"
#include "RTSPMessage.h"
#include "Header.h"
#include "LinkedList.h"
#include "nomad_log.h"

static const char* TAG = "RTSP";

/** @brief RTSPMessage 생성자. rtsp message의 형태를 제공한다.
	@return 의미 없음.
	@param 없음.
*/
RTSPMessage::RTSPMessage(void) :
majorVersion(1),
minorVersion(0),
contentLength(0),
content(NULL)
{
	headerList = new LinkedList(FALSE);
}

/** @brief RTSPMessage 소멸자. rtsp message의 형태를 제공한다.
	@return 의미 없음.
	@param 없음.
*/
RTSPMessage::~RTSPMessage(void)
{
	clear();
	delete headerList;
}

/** @brief RTSPMessage의 rtsp version을 가져온다.
	@return S_OK
	@param pMajorVersion major 버전값을 채운다.
	@param pMinorVersion minor 버전값을 채운다.
*/
HRESULT RTSPMessage::getVersion(int* pMajorVersion, int* pMinorVersion)
{
	*pMajorVersion = majorVersion;
	*pMinorVersion = minorVersion;

	return S_OK;
}

/** @brief RTSPMessage의 rtsp version을 설정한다.
	@return S_OK
	@param pMajorVersion major 버전값을 설정한다.
	@param pMinorVersion minor 버전값을 설정한다.
*/
HRESULT RTSPMessage::setVersion(int nMajorVersion, int nMinorVersion)
{
	majorVersion = nMajorVersion;
	minorVersion = nMinorVersion;
	
	return S_OK;
}

/** @brief RTSPMessage의 clear한다.(메모리 해제등을 수행)
	@return S_OK
	@param 없음.
*/
HRESULT RTSPMessage::clear()
{
	releaseHeaders();
	releaseContent();

	return S_OK;
}

/** @brief RTSPMessage에 Header를 추가 한다.
	@return S_OK
	@param pHeader 추가할 Header
*/
HRESULT RTSPMessage::addHeader(Header* pHeader)
{
	HRESULT hr;

	hr = headerList->addTail(pHeader);
	pHeader->addRef();

	char* szValue = pHeader->getStringValue();

	if(szValue)
	{
		LOGV(TAG, "%s:%d %s: %s", __FILENAME__, __LINE__, rtspGetHeaderName(pHeader->getName()), szValue);
	}
	else
	{
		LOGV(TAG, "%s:%d %s: %d", __FILENAME__, __LINE__, rtspGetHeaderName(pHeader->getName()), pHeader->getIntValue());
	}

	return hr;
}

/** @brief RTSPMessage에서 Header를 가져온다.
	@return S_OK
	@param nHeader 가져올 header의 index
	@param ppHeader 가져온 header를 채움.
*/
HRESULT RTSPMessage::getHeader(int nHeader, Header** ppHeader)
{
	//HRESULT hr;

	int i;
	int nCount;
	Header* pHeader;
	headerList->getCount(&nCount);

	for(i=0; i<nCount; i++)
	{
		headerList->get(i, (void**) &pHeader);

		if(nHeader == pHeader->getName())
		{
			*ppHeader = pHeader;
			return S_OK;
		}
	}

	return E_FAIL;
}

/** @brief RTSPMessage에서 Header를 제거한다.
	@return S_OK
	@param nHeader 제거할 header의 index
	@param ppHeader 제거한 header를 채움.
*/
HRESULT RTSPMessage::removeHeader(int nHeader, Header** ppHeader)
{
	//HRESULT hr;

	int i;
	int nCount;
	Header* pHeader;
	headerList->getCount(&nCount);

	for(i=0; i<nCount; i++)
	{
		headerList->get(i, (void**) &pHeader);

		if(nHeader == pHeader->getName())
		{
			*ppHeader = pHeader;

			return S_OK;
		}
	}

	return E_FAIL;
}

int RTSPMessage::getContentLength()
{
	return contentLength;
}

#if 0
HRESULT CRTSPMessage::SetContentLength(int nContentLength)
{
	HRESULT hr;
	CHeader* pHeader;

	hr = GetHeader(eRH_CONTENT_LENGTH, &pHeader);

	if(FAILED(hr))
	{
	}
	else
	{
	}
}
#endif

/** @brief RTSPMessage에서 body와 body length를 가져온다.
	@return S_OK
	@param pContentLength body의 length를 채움
	@param ppContent body를 채움.
*/
HRESULT RTSPMessage::getContent(char** ppContent, int* pContentLength)
{
	*ppContent = content;
	*pContentLength = contentLength;

	return S_OK;
}

/**
 * pSource may be not a string.
 * @param nLength: ContentLength including '\0'.
 */
/** @brief RTSPMessage에서 body와 body length를 설정한다.
	@return S_OK or E_FAIL
	@param nLength body의 length를 설정
	@param pSource body를 설정
*/
HRESULT RTSPMessage::setContent(const char* pSource, int nLength)
{
	releaseContent();

	if(nLength <= 0)
	{
		return S_FALSE;
	}

	char* pDest = new char [nLength];

	memcpy(pDest, pSource, nLength);

	content = pDest;
	contentLength = nLength;

	return S_OK;
}

/** @brief RTSPMessage에서 body를 삭제하고 메모리를 해제한다.
	@return 없음.
	@param 없음.
*/
void RTSPMessage::releaseContent()
{
	if(content)
	{
		delete[] content;

		content = NULL;
		contentLength = 0;
	}
}

/** @brief RTSPMessage에서 header를 삭제하고 메모리를 해제한다.
	@return 없음.
	@param 없음.
*/
void RTSPMessage::releaseHeaders()
{
	//HRESULT hr;

	int i;
	int nCount;
	Header* pHeader;
	headerList->getCount(&nCount);

	for(i=0; i<nCount; i++)
	{
		headerList->get(i, (void**) &pHeader);
		pHeader->release();
	}

	headerList->clear();
}

/** @brief RTSPMessage에서 header를 가져온다.
	@return S_OK
	@param szBuf header를 채울 버퍼
	@param nBufSize header를 채울 버퍼 크기
*/
HRESULT RTSPMessage::getHeaders(char* szBuf, int nBufSize)
{
	HRESULT hr;
	int nCount;

	Header* pHeader;
	char szLine[256];

	szBuf[0] = '\0';
	hr = headerList->getCount(&nCount);

	for(int i=0; i<nCount; i++)
	{
		hr = headerList->get(i, (void**) &pHeader);

#if 1
		hr = pHeader->toString(szLine, 256);
#else
		szName = (char*) GetHeaderName(pHeader->getName());
		szValue = pHeader->getStringValue();
		if(szValue)
		{
			sprintf(szLine, "%s: %s\r\n", szName, szValue);
		}
		else
		{
			nValue = pHeader->getIntValue();
			sprintf(szLine, "%s: %d\r\n", szName, nValue);
		}
#endif
		
		if(SUCCEEDED(hr))
		{
			strcat(szBuf, szLine);
		}
	}

	// Empty line means that header section is over and next section is content.
	strcat(szBuf, "\r\n");

	LOGV("TRACE", "%s:%d ------------ DATA -----------\n%s-------------",
		__FILENAME__, __LINE__, szBuf);

	return S_OK;
}

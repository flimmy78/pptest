#include "nomad.h"
#include "rtsp.h"
#include "RTSPMessage.h"
#include "Header.h"
#include "LinkedList.h"
#include "nomad_log.h"

static const char* TAG = "RTSP";

/** @brief RTSPMessage ������. rtsp message�� ���¸� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPMessage::RTSPMessage(void) :
majorVersion(1),
minorVersion(0),
contentLength(0),
content(NULL)
{
	headerList = new LinkedList(FALSE);
}

/** @brief RTSPMessage �Ҹ���. rtsp message�� ���¸� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPMessage::~RTSPMessage(void)
{
	clear();
	delete headerList;
}

/** @brief RTSPMessage�� rtsp version�� �����´�.
	@return S_OK
	@param pMajorVersion major �������� ä���.
	@param pMinorVersion minor �������� ä���.
*/
HRESULT RTSPMessage::getVersion(int* pMajorVersion, int* pMinorVersion)
{
	*pMajorVersion = majorVersion;
	*pMinorVersion = minorVersion;

	return S_OK;
}

/** @brief RTSPMessage�� rtsp version�� �����Ѵ�.
	@return S_OK
	@param pMajorVersion major �������� �����Ѵ�.
	@param pMinorVersion minor �������� �����Ѵ�.
*/
HRESULT RTSPMessage::setVersion(int nMajorVersion, int nMinorVersion)
{
	majorVersion = nMajorVersion;
	minorVersion = nMinorVersion;
	
	return S_OK;
}

/** @brief RTSPMessage�� clear�Ѵ�.(�޸� �������� ����)
	@return S_OK
	@param ����.
*/
HRESULT RTSPMessage::clear()
{
	releaseHeaders();
	releaseContent();

	return S_OK;
}

/** @brief RTSPMessage�� Header�� �߰� �Ѵ�.
	@return S_OK
	@param pHeader �߰��� Header
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

/** @brief RTSPMessage���� Header�� �����´�.
	@return S_OK
	@param nHeader ������ header�� index
	@param ppHeader ������ header�� ä��.
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

/** @brief RTSPMessage���� Header�� �����Ѵ�.
	@return S_OK
	@param nHeader ������ header�� index
	@param ppHeader ������ header�� ä��.
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

/** @brief RTSPMessage���� body�� body length�� �����´�.
	@return S_OK
	@param pContentLength body�� length�� ä��
	@param ppContent body�� ä��.
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
/** @brief RTSPMessage���� body�� body length�� �����Ѵ�.
	@return S_OK or E_FAIL
	@param nLength body�� length�� ����
	@param pSource body�� ����
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

/** @brief RTSPMessage���� body�� �����ϰ� �޸𸮸� �����Ѵ�.
	@return ����.
	@param ����.
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

/** @brief RTSPMessage���� header�� �����ϰ� �޸𸮸� �����Ѵ�.
	@return ����.
	@param ����.
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

/** @brief RTSPMessage���� header�� �����´�.
	@return S_OK
	@param szBuf header�� ä�� ����
	@param nBufSize header�� ä�� ���� ũ��
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

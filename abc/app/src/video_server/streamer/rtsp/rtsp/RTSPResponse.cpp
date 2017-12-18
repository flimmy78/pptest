#include "nomad.h"
#include "RTSPResponse.h"
#include "LinkedList.h"
#include "Header.h"
#include "rtsp.h"

#if 0
typedef struct _SStatusCodeAndReason
{
	int nCode;
	const char* szReason;

} SStatusCodeAndReason;

const SStatusCodeAndReason STATUS_CODE_TABLE[] =
{
	{ 100, "Continue" },
	{ 200, "OK" },
	{ 400, "Bad Request" },
	{ 401, "Unauthorized" },
	{ 404, "Not Found" },

	{ 500, "Internal Server Error" },
	{ 501, "Not Implemented" },
	{ 505, "RTSP Version Not Supported" },
	{ 551, "Option not support" },
};

static const char* GetStatusReason(int nStatusCode)
{
	int nCount = sizeof(STATUS_CODE_TABLE) / sizeof(STATUS_CODE_TABLE[0]);

	for(int i=0; i<nCount; i++)
	{
		if(STATUS_CODE_TABLE[i].nCode == nStatusCode)
		{
			return STATUS_CODE_TABLE[i].szReason;
		}
	}

	return NULL;
}
#endif

/** @brief RTSPResponse 생성자. rtsp response message의 형태를 제공한다.
	@return 의미 없음.
	@param 없음.
*/
RTSPResponse::RTSPResponse(void)
{
}

/** @brief RTSPResponse 소멸자. rtsp response message의 형태를 제공한다.
	@return 의미 없음.
	@param 없음.
*/
RTSPResponse::~RTSPResponse(void)
{
}

/** @brief rtsp response의 return code를 가져온다.
	@return rtsp response의 return code(200, 404, ...)
	@param 없음.
*/
int RTSPResponse::getStatusCode()
{
	return statusCode;
}

/** @brief rtsp response의 return code를 설정한다.
	@return S_OK
	@param nStatusCode 설정할 rtsp response code(200, 404, ...)
*/
HRESULT RTSPResponse::setStatusCode(int nStatusCode)
{
	statusCode = nStatusCode;
	return S_OK;
}

/** @brief 설정된 response code를 가지고 response string 형태로 만들어준다.
	@return S_OK
	@param szBuf response string 형태를 저장할 버퍼
	@param nBufSize response string 형태를 저장할 버퍼 크기
*/
HRESULT RTSPResponse::getStatusLine(char* szBuf, int nBufSize)
{
	const char* szReason = rtspGetStatusReason(statusCode);

	sprintf(szBuf, "RTSP/%d.%d %d %s\r\n",
		majorVersion, minorVersion,
		statusCode, szReason);

	return S_OK;
}

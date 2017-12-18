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

/** @brief RTSPResponse ������. rtsp response message�� ���¸� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPResponse::RTSPResponse(void)
{
}

/** @brief RTSPResponse �Ҹ���. rtsp response message�� ���¸� �����Ѵ�.
	@return �ǹ� ����.
	@param ����.
*/
RTSPResponse::~RTSPResponse(void)
{
}

/** @brief rtsp response�� return code�� �����´�.
	@return rtsp response�� return code(200, 404, ...)
	@param ����.
*/
int RTSPResponse::getStatusCode()
{
	return statusCode;
}

/** @brief rtsp response�� return code�� �����Ѵ�.
	@return S_OK
	@param nStatusCode ������ rtsp response code(200, 404, ...)
*/
HRESULT RTSPResponse::setStatusCode(int nStatusCode)
{
	statusCode = nStatusCode;
	return S_OK;
}

/** @brief ������ response code�� ������ response string ���·� ������ش�.
	@return S_OK
	@param szBuf response string ���¸� ������ ����
	@param nBufSize response string ���¸� ������ ���� ũ��
*/
HRESULT RTSPResponse::getStatusLine(char* szBuf, int nBufSize)
{
	const char* szReason = rtspGetStatusReason(statusCode);

	sprintf(szBuf, "RTSP/%d.%d %d %s\r\n",
		majorVersion, minorVersion,
		statusCode, szReason);

	return S_OK;
}

#include "nomad.h"
#include "rtsp.h"
#include "RangeHeader.h"
#include "nomad_log.h"

static const char* TAG = "RTSP";

/** @brief RangeHeader(rtsp header) ������. 10-100 ������ value ���� ����
	@return �ǹ� ����.
	@param ����.
*/
RangeHeader::RangeHeader(void) :
Header(eRH_RANGE),
startTime(-1.0),
endTime(-1.0)
{
}

/** @brief RangeHeader(rtsp header) �Ҹ���.
	@return �ǹ� ����.
	@param ����.
*/
RangeHeader::~RangeHeader(void)
{
}

/** @brief rangeheader�� start time�� �����´�. 10-100���� 10�� ��
	@return rangeheader�� start time
	@param ����.
*/
double RangeHeader::getStartTime() const
{
	return startTime;
}

/** @brief rangeheader�� end time�� �����´�. 10-100���� 100�� ��
	@return rangeheader�� end time
	@param ����.
*/
double RangeHeader::getEndTime() const
{
	return endTime;
}

/** @brief rangeheader�� start time�� �����Ѵ�.. 10-100���� 10�� ��
	@return ����.
	@param time rangeheader�� start time
*/
void RangeHeader::setStartTime(double time)
{
	startTime = time;
}

/** @brief rangeheader�� end time�� �����Ѵ�.. 10-100���� 100�� ��
	@return ����.
	@param time rangeheader�� end time
*/
void RangeHeader::setEndTime(double time)
{
	endTime = time;
}

/** @brief rangeheader�� start, end time�� �����Ѵ�.
	@return ����.
	@param startTime rangeheader�� start time
	@param endTime rangeheader�� end time
*/
void RangeHeader::setTime(double startTime, double endTime)
{
	this->startTime = startTime;
	this->endTime = endTime;
}

/** @brief rangeheader value�� �Ľ��� start, end time�� �����Ѵ�.
	@return rangeheaer format�� ������ S_OK, �ƴϸ� E_FAIL
	@param headerValue �Ľ��� rangeheader�� string value
*/
HRESULT RangeHeader::parse(char* headerValue)
{
	LOGD_CALL(TAG, "[START]");

	// Only npt is supported here.
	const char* formatList[] = { "npt", "smpte", "clock" };

	char* tmpText = strchr(headerValue, '=');
	*tmpText = '\0';

	char* timeFormat = headerValue;

	// Normal Play Time
	int i;
	for(i=0; i<3; i++)
	{
		if(strcmp(timeFormat, formatList[i]) == 0)
		{
			// Only npt is supported here.
			break;
		}
	}

	if(i != 0)
	{
		return E_NOTIMPL;
	}
	
	// Parse a header value as npt time format.
	// Time value part
	//HRESULT hr;
	char* tmpTime = tmpText + 1;

	LOGD_CALL(TAG, "");
	tmpText = strchr(tmpTime, '-');
	if(tmpText == NULL)
	{
		LOGE("ERROR", "%s:%d Wrong npt format: %s",
			__FILENAME__, __LINE__, tmpTime);
		return E_FAIL;
	}

	*tmpText = '\0';

	LOGD_CALL(TAG, "");
	parseNormalPlayTime(tmpTime, &startTime);
	LOGD(TAG, "%s:%d StartTime: %.3f", __FILENAME__, __LINE__, startTime);

	// EndTime
	LOGD_CALL(TAG, "");
	tmpTime = tmpText + 1;
	parseNormalPlayTime(tmpTime, &endTime);
	LOGD(TAG, "%s:%d EndTime: %.3f", __FILENAME__, __LINE__, endTime);

	LOGI(TAG, "%s:%d Range: %.3f - %.3f", __FILENAME__, __LINE__, startTime, endTime);
	LOGD_CALL(TAG, "[END]");

	return S_OK;
}

Header* RangeHeader::duplicate() const
{
	return NULL;
}

/** @brief RangeHeader�� string ���� �����.
	@return RangeHeader == null E_FAIL, �ƴϸ� S_OK
	@param buffer string�� ������ ����
	@param bufferSize string�� ������ ������ ũ��
*/
HRESULT RangeHeader::toString(char* buffer, int bufferSize)
{
	const char* headerName = rtspGetHeaderName(name);
	if(headerName == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr;
	char tmpTime[64];

	sprintf(buffer, "%s: npt=", headerName);

	if(startTime > -1.0)
	{
		hr = convertTimeToNPTFormat(startTime, tmpTime);
		if(SUCCEEDED(hr))
		{
			strcat(buffer, tmpTime);
		}
	}

	strcat(buffer, "-");

//	if(endTime >= 0.0)
//	{
//		hr = convertTimeToNPTFormat(endTime, tmpTime);
//		if(SUCCEEDED(hr))
//		{
//			strcat(buffer, tmpTime);
//		}
//	}

	strcat(buffer, "\r\n");

	return S_OK;
}

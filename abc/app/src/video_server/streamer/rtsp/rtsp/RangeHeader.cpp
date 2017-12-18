#include "nomad.h"
#include "rtsp.h"
#include "RangeHeader.h"
#include "nomad_log.h"

static const char* TAG = "RTSP";

/** @brief RangeHeader(rtsp header) 생성자. 10-100 형태의 value 값을 가짐
	@return 의미 없음.
	@param 없음.
*/
RangeHeader::RangeHeader(void) :
Header(eRH_RANGE),
startTime(-1.0),
endTime(-1.0)
{
}

/** @brief RangeHeader(rtsp header) 소멸자.
	@return 의미 없음.
	@param 없음.
*/
RangeHeader::~RangeHeader(void)
{
}

/** @brief rangeheader의 start time을 가져온다. 10-100에서 10의 값
	@return rangeheader의 start time
	@param 없음.
*/
double RangeHeader::getStartTime() const
{
	return startTime;
}

/** @brief rangeheader의 end time을 가져온다. 10-100에서 100의 값
	@return rangeheader의 end time
	@param 없음.
*/
double RangeHeader::getEndTime() const
{
	return endTime;
}

/** @brief rangeheader의 start time을 설정한다.. 10-100에서 10의 값
	@return 없음.
	@param time rangeheader의 start time
*/
void RangeHeader::setStartTime(double time)
{
	startTime = time;
}

/** @brief rangeheader의 end time을 설정한다.. 10-100에서 100의 값
	@return 없음.
	@param time rangeheader의 end time
*/
void RangeHeader::setEndTime(double time)
{
	endTime = time;
}

/** @brief rangeheader의 start, end time을 설정한다.
	@return 없음.
	@param startTime rangeheader의 start time
	@param endTime rangeheader의 end time
*/
void RangeHeader::setTime(double startTime, double endTime)
{
	this->startTime = startTime;
	this->endTime = endTime;
}

/** @brief rangeheader value를 파싱해 start, end time을 설정한다.
	@return rangeheaer format에 맞으면 S_OK, 아니면 E_FAIL
	@param headerValue 파싱할 rangeheader의 string value
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

/** @brief RangeHeader를 string 으로 만든다.
	@return RangeHeader == null E_FAIL, 아니면 S_OK
	@param buffer string이 저정될 버퍼
	@param bufferSize string이 저정될 버퍼의 크기
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

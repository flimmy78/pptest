#include "nomad.h"
#include "Header.h"
#include "rtsp.h"
#include "TransportHeader.h"
#include "RangeHeader.h"
#include <string.h>

//static const char* TAG = "HEADER";

/** @brief Header(rtsp header) 생성자.
	@return 의미 없음.
	@param 없음.
*/
Header::Header(void) :
name(-1),
integerValue(-1),
stringValue(NULL)
{
}

/** @brief Header(rtsp header) 생성자.
	@return 의미 없음.
	@param nName header table의 index
*/
Header::Header(int nName) :
name(nName),
integerValue(-1),
stringValue(NULL)
{
}

/** @brief Header(rtsp header) 소멸자
	@return 의미 없음.
	@param 없음.
*/
Header::~Header(void)
{
	//HRESULT hr;
	::releaseString(&stringValue);
}

/** @brief Header의 key(RTP-Info, Scale, Session 등)의 index를 가져온다.
	@return header의 key index
	@param 없음.
*/
int Header::getName()
{
	return name;
}

/** @brief Header의 key(RTP-Info, Scale, Session 등)의 index로 새로운 rtsp header를 만든다.
	@return 새로 만들어진 Header* (rtsp header)
	@param key RTP-Info, Scale, Session 등의 index
*/
Header* Header::create(int key)
{
	Header* header = NULL;

	switch(key)
	{
	case eRH_TRANSPORT:
		header = new TransportHeader();
		break;
	case eRH_RANGE:
		header = new RangeHeader();
		break;
	default:
		header = new Header();
	}

	return header;
}

/** @brief Header의 key(RTP-Info, Scale, Session 등)의 index를 설정한다.
	@return S_OK
	@param key Header를 만들 RTP-Info, Scale, Session 등의 index
*/
HRESULT Header::setName(int key)
{
	name = key;
	return S_OK;
}

/** @brief Header의 integer value값을 가져온다.
	@return 가져온 integer value 값
	@param 없음.
*/
int Header::getIntValue()
{
	return integerValue;
}

/** @brief Header의 integer value값을 설정한다.
	@return S_OK
	@param 설정할 integer value 값
*/
HRESULT Header::setIntValue(int value)
{
	integerValue = value;
	return S_OK;
}

/** @brief Header의 string value값을 설정한다.
	@return 설정할 source == null E_FAIL, 아니면 S_OK
	@param source 설정할 string
*/
HRESULT Header::setStringValue(const char* source)
{
	if(source == NULL)
	{
		return E_FAIL;
	}

	HRESULT hr;
	char value[256];

	strcpy(value, source);

	hr = ::releaseString(&stringValue);
	hr = ::copyStringWithAllocation(value, &stringValue);

	hr = parse(value);
	
	return hr;
}

/** @brief Header의 string value값을 가져온다.
	@return 가져온 string value 값
	@param 없음.
*/
char* Header::getStringValue()
{
	return stringValue;
}

/** @brief Header를 복사한다.
	@return 복사된 Header
	@param 없음.
*/
Header* Header::duplicate() const
{
	Header* header = new Header();

	header->setName(name);
	header->setIntValue(integerValue);
	header->setStringValue(stringValue);

	return header;
}

/** @brief Header를 string 으로 만든다.
	@return header == null E_FAIL, 아니면 S_OK
	@param buffer string이 저정될 버퍼
	@param bufferSize string이 저정될 버퍼의 크기
*/
HRESULT Header::toString(char* buffer, int bufferSize)
{
	const char* headerName = rtspGetHeaderName(name);
	if(headerName == NULL)
	{
		return E_FAIL;
	}

	if(stringValue)
	{
		sprintf(buffer, "%s: %s\r\n", headerName, stringValue);
	}
	else
	{
		sprintf(buffer, "%s: %d\r\n", headerName, integerValue);
	}

	return S_OK;
}

HRESULT Header::parse(char* headerValue)
{
	return S_OK;
}

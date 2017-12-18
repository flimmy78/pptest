#include "nomad.h"
#include "Header.h"
#include "rtsp.h"
#include "TransportHeader.h"
#include "RangeHeader.h"
#include <string.h>

//static const char* TAG = "HEADER";

/** @brief Header(rtsp header) ������.
	@return �ǹ� ����.
	@param ����.
*/
Header::Header(void) :
name(-1),
integerValue(-1),
stringValue(NULL)
{
}

/** @brief Header(rtsp header) ������.
	@return �ǹ� ����.
	@param nName header table�� index
*/
Header::Header(int nName) :
name(nName),
integerValue(-1),
stringValue(NULL)
{
}

/** @brief Header(rtsp header) �Ҹ���
	@return �ǹ� ����.
	@param ����.
*/
Header::~Header(void)
{
	//HRESULT hr;
	::releaseString(&stringValue);
}

/** @brief Header�� key(RTP-Info, Scale, Session ��)�� index�� �����´�.
	@return header�� key index
	@param ����.
*/
int Header::getName()
{
	return name;
}

/** @brief Header�� key(RTP-Info, Scale, Session ��)�� index�� ���ο� rtsp header�� �����.
	@return ���� ������� Header* (rtsp header)
	@param key RTP-Info, Scale, Session ���� index
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

/** @brief Header�� key(RTP-Info, Scale, Session ��)�� index�� �����Ѵ�.
	@return S_OK
	@param key Header�� ���� RTP-Info, Scale, Session ���� index
*/
HRESULT Header::setName(int key)
{
	name = key;
	return S_OK;
}

/** @brief Header�� integer value���� �����´�.
	@return ������ integer value ��
	@param ����.
*/
int Header::getIntValue()
{
	return integerValue;
}

/** @brief Header�� integer value���� �����Ѵ�.
	@return S_OK
	@param ������ integer value ��
*/
HRESULT Header::setIntValue(int value)
{
	integerValue = value;
	return S_OK;
}

/** @brief Header�� string value���� �����Ѵ�.
	@return ������ source == null E_FAIL, �ƴϸ� S_OK
	@param source ������ string
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

/** @brief Header�� string value���� �����´�.
	@return ������ string value ��
	@param ����.
*/
char* Header::getStringValue()
{
	return stringValue;
}

/** @brief Header�� �����Ѵ�.
	@return ����� Header
	@param ����.
*/
Header* Header::duplicate() const
{
	Header* header = new Header();

	header->setName(name);
	header->setIntValue(integerValue);
	header->setStringValue(stringValue);

	return header;
}

/** @brief Header�� string ���� �����.
	@return header == null E_FAIL, �ƴϸ� S_OK
	@param buffer string�� ������ ����
	@param bufferSize string�� ������ ������ ũ��
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

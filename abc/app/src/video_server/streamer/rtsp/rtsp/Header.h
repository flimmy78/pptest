#ifndef __HEADER_H__
#define __HEADER_H__

#include "RefCount.h"

class Header
{
	DECLARE_REF_COUNT

protected:
	int name;
	int integerValue;
	char* stringValue;

protected:
	//virtual HRESULT Release();
	virtual HRESULT parse(char* headerValue);

public:
	Header(void);
	Header(int name);
	virtual ~Header(void);

	static Header* create(int header);

	int getName();
	HRESULT setName(int name);

	int getIntValue();
	HRESULT setIntValue(int value);

	HRESULT setStringValue(const char* value);
	char* getStringValue();

	virtual Header* duplicate() const;
	virtual HRESULT toString(char* buffer, int bufferSize);
};

#endif	// __HEADER_H__

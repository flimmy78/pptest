#ifndef __RANGE_HEADER_H__
#define __RANGE_HEADER_H__

#include "Header.h"

class RangeHeader : public Header
{
protected:
	double startTime;
	double endTime;

protected:
	virtual HRESULT parse(char* headerValue);

public:
	RangeHeader(void);
	virtual ~RangeHeader(void);

	double getStartTime() const;
	double getEndTime() const;

	void setStartTime(double time);
	void setEndTime(double time);
	void setTime(double startTime, double endTime);

	virtual Header* duplicate() const;
	virtual HRESULT toString(char* buffer, int bufferSize);
};

#endif // __RANGE_HEADER_H__

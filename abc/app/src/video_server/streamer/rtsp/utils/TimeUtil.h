#ifndef GW_TIME_UTIL_H
#define GW_TIME_UTIL_H

#include "gwutilsbase.h"

GW_NAMESPACE_BEGIN

void msleep(long msec);
void nsleep(int64_t nanosecond);
unsigned long long getTimeMS();
unsigned long long getTimeMicroSecond();
unsigned long long getTimeNanoSecond();

unsigned int convertTSToMS(long long nTimeStamp, int nSampleRate);
unsigned int convertTSToMicroSecond(long long nTimeStamp, int nSampleRate);
long long convertTSToNS(long long nTimeStamp, int nSampleRate);

GW_NAMESPACE_END

#endif
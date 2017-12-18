#include "gwutilsbase.h"

#if defined(WIN32)

#include <Windows.h>

#elif OSX

#include <sys/time.h>

#elif LINUX

#include <sys/time.h>
#include <time.h>

#endif

GW_NAMESPACE_BEGIN

/**
 * return msecs.
 */
/** @brief 현재 시간을 milli sec로 알아온다.
	@return 현재 milli sec
	@param 없음.
*/
unsigned long long getTimeMS()
{
	unsigned long long msec;

#ifdef WIN32

	msec = ::GetTickCount();

#else

	struct timeval now;
	gettimeofday(&now, NULL);
	msec = now.tv_sec * 1000ULL + now.tv_usec / 1000ULL;

#endif

	return msec;
}

/**
 * return micro seconds.
 */
/** @brief 현재 시간을 micro sec로 알아온다.
	@return 현재 micro sec
	@param 없음.
*/
unsigned long long getTimeMicroSecond()
{
	unsigned long long msec;

	struct timeval now;
	gettimeofday(&now, NULL);
	msec = now.tv_sec * 1000000ULL + now.tv_usec;

	return msec;
}

/**
 * return micro seconds.
 */
/** @brief 현재 시간을 nano sec로 알아온다.
	@return 현재 nano sec
	@param 없음.
*/
unsigned long long getTimeNanoSecond()
{
	unsigned long long msec;

	struct timeval now;
	gettimeofday(&now, NULL);
	msec = now.tv_sec * 1000000000ULL + now.tv_usec*1000;

	return msec;
}

/** @brief milli sec로 sleep 한다.
	@return 없음.
	@param msec sleep 할 milli sec
*/
void msleep(long msec)
{
#ifdef WIN32

	Sleep(msec);

#else

    struct timespec ts;
    long sec = msec / 1000;
    long nsec = (msec % 1000) * 1000000;

    ts.tv_sec = sec;
    ts.tv_nsec = nsec;

    nanosleep(&ts, NULL);

#endif
}

#ifndef WIN32
/** @brief nano sec로 sleep 한다.
	@return 없음.
	@param nanosecond sleep 할 nano sec
*/
void nsleep(int64_t nanosecond)
{
    struct timespec ts;
    long sec  = (time_t) (nanosecond / 1000000000LL);
    long nsec = (long)   (nanosecond % 1000000000LL);

    ts.tv_sec = sec;
    ts.tv_nsec = nsec;

    nanosleep(&ts, NULL);
}

#endif

/**
 * @param nTimeStamp: unit(hz)
 * @param nSampleRate
 */
/** @brief timestamp와 samplerate 를 가지고 milli sec 시간을 만든다.
	@return 만든 milli sec 시간
	@param nTimeStamp 
	@param nSampleRate
*/
unsigned int convertTSToMS(long long nTimeStamp, int nSampleRate)
{
#if 0

	long long llTime = (nTimeStamp * 1000LL) / m_nSampleRate;
	return (unsigned int) llTime;

#else

	return (unsigned int) (1000.0 * ((double) nTimeStamp / (double) nSampleRate));

#endif
}

/**
 * @param nTimeStamp: unit(hz)
 * @param nSampleRate
 */
/** @brief timestamp와 samplerate 를 가지고 nano sec 시간을 만든다.
	@return 만든 nano sec 시간
	@param nTimeStamp 
	@param nSampleRate
*/
long long convertTSToNS(long long nTimeStamp, int nSampleRate)
{
	long long llTime = (nTimeStamp * 1000000000LL) / nSampleRate;
	return llTime;
}

/**
 * @param nTimeStamp: unit(hz)
 * @param nSampleRate
 */
/** @brief timestamp와 samplerate 를 가지고 micro sec 시간을 만든다.
	@return 만든 micro sec 시간
	@param nTimeStamp 
	@param nSampleRate
*/
long long convertTSToMicroSecond(long long nTimeStamp, int nSampleRate)
{
	long long llTime = (nTimeStamp * 1000000LL) / nSampleRate;
	return llTime;
}

GW_NAMESPACE_END

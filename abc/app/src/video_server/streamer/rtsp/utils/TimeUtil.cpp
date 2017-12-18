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
/** @brief ���� �ð��� milli sec�� �˾ƿ´�.
	@return ���� milli sec
	@param ����.
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
/** @brief ���� �ð��� micro sec�� �˾ƿ´�.
	@return ���� micro sec
	@param ����.
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
/** @brief ���� �ð��� nano sec�� �˾ƿ´�.
	@return ���� nano sec
	@param ����.
*/
unsigned long long getTimeNanoSecond()
{
	unsigned long long msec;

	struct timeval now;
	gettimeofday(&now, NULL);
	msec = now.tv_sec * 1000000000ULL + now.tv_usec*1000;

	return msec;
}

/** @brief milli sec�� sleep �Ѵ�.
	@return ����.
	@param msec sleep �� milli sec
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
/** @brief nano sec�� sleep �Ѵ�.
	@return ����.
	@param nanosecond sleep �� nano sec
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
/** @brief timestamp�� samplerate �� ������ milli sec �ð��� �����.
	@return ���� milli sec �ð�
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
/** @brief timestamp�� samplerate �� ������ nano sec �ð��� �����.
	@return ���� nano sec �ð�
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
/** @brief timestamp�� samplerate �� ������ micro sec �ð��� �����.
	@return ���� micro sec �ð�
	@param nTimeStamp 
	@param nSampleRate
*/
long long convertTSToMicroSecond(long long nTimeStamp, int nSampleRate)
{
	long long llTime = (nTimeStamp * 1000000LL) / nSampleRate;
	return llTime;
}

GW_NAMESPACE_END

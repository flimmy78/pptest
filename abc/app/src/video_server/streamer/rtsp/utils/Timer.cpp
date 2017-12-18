#include "Timer.h"
#include "TimeUtil.h"

GW_NAMESPACE_BEGIN

/** @brief Timer 생성자. 특정 시간이 경과했는지 알아온다.
	@return 의미 없음.
	@param 없음.
*/
Timer::Timer(void) :
startTime(0),
alarmTime(0)
{
}

/** @brief Timer 생성자. 특정 시간이 경과했는지 알아온다.
	@return 의미 없음.
	@param msTimeout alarm 시간
*/
Timer::Timer(int msTimeout)
{
	set(msTimeout);
}

/** @brief Timer 소멸자
	@return 의미 없음.
	@param 없음.
*/
Timer::~Timer(void)
{
}

/** @brief alarm 시간을 설정한다.
	@return 의미 없음.
	@param msTimeout alarm 시간
*/
void Timer::set(int msTimeout)
{
	startTime = getTimeMS();
	alarmTime = startTime + msTimeout;
}

/** @brief 설정된 alarm 시간이 경과 했는지 확인한다.
	@return alarm 시간이 경과 했으면 true, 아니면 false
	@param remainingTime alarm 시간이 경과하지 않았으면 남은 시간
*/
bool Timer::alarm(unsigned long long* remainingTime)
{
	bool isAlarm;
	unsigned long long remaining_time = 0ull;
	unsigned long long curTime = getTimeMS();

#ifdef WIN32

	// GetTickCount() will wrap around to zero if the system is run continuously for 49.7 days.
	if(curTime < startTime)
	{
		curTime += 0x100000000ULL;
	}

#endif

	isAlarm = (curTime >= alarmTime);

	if( !isAlarm )
	{
		remaining_time = alarmTime - curTime;
	}

	if(remainingTime)
	{
		*remainingTime = remaining_time;
	}

	return isAlarm;
}
GW_NAMESPACE_END

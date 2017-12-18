#include "Timer.h"
#include "TimeUtil.h"

GW_NAMESPACE_BEGIN

/** @brief Timer ������. Ư�� �ð��� ����ߴ��� �˾ƿ´�.
	@return �ǹ� ����.
	@param ����.
*/
Timer::Timer(void) :
startTime(0),
alarmTime(0)
{
}

/** @brief Timer ������. Ư�� �ð��� ����ߴ��� �˾ƿ´�.
	@return �ǹ� ����.
	@param msTimeout alarm �ð�
*/
Timer::Timer(int msTimeout)
{
	set(msTimeout);
}

/** @brief Timer �Ҹ���
	@return �ǹ� ����.
	@param ����.
*/
Timer::~Timer(void)
{
}

/** @brief alarm �ð��� �����Ѵ�.
	@return �ǹ� ����.
	@param msTimeout alarm �ð�
*/
void Timer::set(int msTimeout)
{
	startTime = getTimeMS();
	alarmTime = startTime + msTimeout;
}

/** @brief ������ alarm �ð��� ��� �ߴ��� Ȯ���Ѵ�.
	@return alarm �ð��� ��� ������ true, �ƴϸ� false
	@param remainingTime alarm �ð��� ������� �ʾ����� ���� �ð�
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

#ifndef GW_TIMER_H
#define GW_TIMER_H

#include <nomad_log.h>

GW_NAMESPACE_BEGIN

class Timer
{
public:
	Timer(void);
	Timer(int msTimeout);
	~Timer(void);

	void set(int msTimeout);
	bool alarm(unsigned long long* RemainingTime = NULL);

private:
	unsigned long long startTime; 

	/**
	 * Unit: ms
	 */
	unsigned long long alarmTime;
};
GW_NAMESPACE_END

#endif

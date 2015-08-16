#include "Lib.h"
#include <time.h>

clock_t timer_timeout = 0;
clock_t timer_start_time = 0;
bool timer_started = false;

bool waitForTimeout()
{
	if(timer_started)
	{
		clock_t now = clock() * 1000.0f / CLOCKS_PER_SEC;
		if(now - timer_start_time > timer_timeout)
		{
			timer_started = false;
			return true;
		}
		return false;
	}
	return true;
}

void setTimeout(int timeout)
{
	timer_timeout = timeout;
}

void startTimer()
{
	timer_start_time = clock() * 1000.0f / CLOCKS_PER_SEC;
	timer_started = true;
}

bool timerStarted()
{
	return timer_started;
}
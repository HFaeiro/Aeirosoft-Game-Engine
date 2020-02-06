#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
	stop = std::chrono::high_resolution_clock::now();
}

double Timer::GetMillisecondsElapsed()
{
	if (isOn)
	{
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start);
		return elapsed.count();
	}
	else
	{
		auto elapsed = std::chrono::duration<double, std::milli>(stop - start);
		return elapsed.count();
	}
}

void Timer::restart()
{
	isOn = true;
	start = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop()
{
	if (!isOn)
		return false;
	else
	{
		stop = std::chrono::high_resolution_clock::now();
		isOn = false;
		return true;
	}
}

bool Timer::Start()
{
	if (isOn)
	{
		return false;
	}
	else
	{
		start = std::chrono::high_resolution_clock::now();
		isOn = true;
		return true;
	}
}

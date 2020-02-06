#pragma once

#include <chrono>

class Timer
{
public:
	Timer();
	double GetMillisecondsElapsed();
	void restart();
	bool Stop();
	bool Start();

private:
	bool isOn = false;
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> stop;

};


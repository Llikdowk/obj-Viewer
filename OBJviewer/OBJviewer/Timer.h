#pragma once

#include <chrono>
#include <ratio>

class Timer {
private:
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::high_resolution_clock::time_point deltaStartTime;
	float deltaTime;
public:
	Timer();
	long long getElapsedTime();
	float getDeltaTime();
	void startDeltaChrono();
	void stopDeltaChrono();
};
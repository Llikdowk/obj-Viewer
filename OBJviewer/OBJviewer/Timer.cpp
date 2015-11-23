#include "Timer.h"

Timer::Timer() {
	startTime = std::chrono::high_resolution_clock::now();
	deltaStartTime = startTime;
	deltaTime = 0.0f;
}

long long Timer::getElapsedTime() {
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);
	return ms.count();
}

float Timer::getDeltaTime() {
	return deltaTime;
}

void Timer::startDeltaChrono() {
	deltaStartTime = std::chrono::high_resolution_clock::now();
}

void Timer::stopDeltaChrono() {
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(now - deltaStartTime).count() / (float)std::nano::den;
}

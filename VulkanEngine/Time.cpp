#include "stdafx.h"
#include "Time.h"

Time::TimePoint Time::startTime = std::chrono::steady_clock::now();

Time::TimePoint Time::currentTime = std::chrono::steady_clock::now();

unsigned int Time::frameCount = 0;

float Time::deltaTime = 0.0f;

void Time::initialize()
{
	startTime	= std::chrono::steady_clock::now();
	frameCount	= 0;
}

void Time::update()
{
	// Protection against integer overflow

	if (frameCount == UINT_MAX)
	{
		initialize();
	}

	currentTime = std::chrono::steady_clock::now();

	std::chrono::duration<float, std::milli> duration = currentTime - startTime;

	// Divide-by-zero protection

	if (frameCount != 0)
	{
		deltaTime = duration.count() / (frameCount * 1000.0f);
	}

	++frameCount;
}

float Time::getFrameRate()
{
	return 1.0f / deltaTime;
}

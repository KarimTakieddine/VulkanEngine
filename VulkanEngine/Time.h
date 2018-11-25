#pragma once

#include "stdafx.h"

class Time
{
public:

	typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;

	static void initialize();
	static void update();

	static float getFrameRate();

	static TimePoint currentTime;
	static TimePoint startTime;
	static unsigned int frameCount;
	static float deltaTime;
};
#pragma once

#include "GlobalHeader.h"

class Time
{
public:
	static void Start();
	static void Update();
	
	static float GetDelta() { return deltaTime; };
	static float GetFPS() { return fps; };
	static float GetTotalRunTime() { return totalRunTime; };

private:
	static INT64 ticksPerSecond;
	static INT64 currentTime;
	static INT64 lastTime;
	static INT64 fpsUpdateInterval;
	static INT64 lastFPSUpdate;
	static UINT framesCount;
	static float totalRunTime;
	static float deltaTime;
	static float fps;
};
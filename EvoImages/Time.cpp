#include "Time.h"

INT64 Time::ticksPerSecond;
INT64 Time::currentTime = 0;
INT64 Time::lastTime;
INT64 Time::fpsUpdateInterval;
INT64 Time::lastFPSUpdate;
UINT Time::framesCount = 0;
float Time::totalRunTime = 0;
float Time::deltaTime = 0;
float Time::fps = 0;

void Time::Start()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond);
	QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	lastFPSUpdate = lastTime;
	fpsUpdateInterval = ticksPerSecond >> 1; //divide by 2
}

void Time::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);

	//updating delta
	deltaTime = (float)(currentTime - lastTime) / (float)ticksPerSecond;
	totalRunTime += deltaTime;

	//updating fps
	framesCount++;
	if(currentTime - lastFPSUpdate > fpsUpdateInterval)
	{
		float time = (float)(currentTime - lastFPSUpdate)/(float)ticksPerSecond;
		fps = (float)framesCount / time;
		lastFPSUpdate = currentTime;
		framesCount = 0;
	}
	lastTime = currentTime;
}

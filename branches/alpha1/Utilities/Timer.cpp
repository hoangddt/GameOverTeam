#include "Timer.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>

float Timer::frameTime = 0;
float Timer::gameTime = 0;
clock_t Timer::deltaTime = 0;
clock_t Timer::lastTime = 0;

Timer::Timer(void)
{
	srand ((unsigned int)time(0));
}
int Timer::GetRand()
{
	return rand();
}
int Timer::GetRandMinMax(int min, int max)
{
	return (rand() % ( max - min + 1)) + min;
}
void Timer::Tick()
{
	clock_t currentTime = clock();
	frameTime = (float)(currentTime - lastTime) / CLOCKS_PER_SEC;
	gameTime += frameTime;
	lastTime = currentTime;
}
void Timer::Reset()
{
	lastTime = clock();
	gameTime = 0;
	frameTime = 0;
}
float Timer::GameTime()
{
	return gameTime;
}
float Timer::FrameTime()
{
	return frameTime;
}
Timer::~Timer(void)
{
}
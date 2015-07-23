#pragma once
#include <time.h>
class Timer
{
	static float frameTime;
	static float gameTime;
	static clock_t lastTime;
	static clock_t deltaTime;
public:
	float static GameTime();
	float static FrameTime();
	int static GetRand();
	int static GetRandMinMax(int min = 0, int max = 100);
	void static Tick();
	void static Reset();
	Timer(void);
	virtual ~Timer(void);
};
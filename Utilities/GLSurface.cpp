#include "ESContext.h"
#include <stddef.h>
#include "Timer.h"

ESContext context_tt;

void RegisterInitFunc(void (*OnInit)(void))
{
	context_tt.OnInit = OnInit;
}

void RegisterDestroyFunc(void (*OnDestroy)(void))
{
	context_tt.OnDestroy = OnDestroy;
}

void RegisterDrawFunc(void (*OnDraw)(void))
{
	context_tt.OnDraw = OnDraw;
}
void RegisterUpdateFunc(void (*OnUpdate)(void))
{
	context_tt.OnUpdate = OnUpdate;
}
void RegisterKeyFunc(void (*OnKey)(int, bool))
{
	context_tt.OnKey = OnKey;
}

void RegisterMouseFunc(void (*OnMouse)(int type, int x, int y))
{
	context_tt.OnMouse = OnMouse;
}

void fireLoopEvent()
{
}

void fireInitEvent()
{
	if(context_tt.OnInit != NULL)
	{
		context_tt.OnInit();
	}
}

void fireDrawEvent()
{
	if(context_tt.OnDraw != NULL)
	{
		context_tt.OnDraw();
	}
}

void fireUpdateEvent()
{
	if(context_tt.OnUpdate != NULL)
	{
		context_tt.OnUpdate();
	}
}

void fireDestroyEvent()
{
	if(context_tt.OnDestroy != NULL)
	{
		context_tt.OnDestroy();
	}
}

void fireMouseEvent(int type, int x, int y)
{
	if(context_tt.OnMouse != NULL)
	{
		context_tt.OnMouse(type, x, y);
	}
}

void fireKeyEvent(int key_code, bool is_down)
{
	if(context_tt.OnKey != NULL)
	{
		context_tt.OnKey(key_code, is_down);
	}
}

void fireTickEvent()
{
	Timer::Tick();
}
#include "Application.h"
#include "../Utilities/Utilities.h"
#include "../lib/vld/vld.h"


void init()
{
	Application::getInstance()->init(700, 700);
}

void update()
{
	Application::getInstance()->update();
}

void draw()
{
	Application::getInstance()->render();
}

void destroy()
{
	Application::getInstance()->destroy();
	Application::getInstance()->destroyInstance();
}

int main()
{
	RegisterInitFunc(init);
	RegisterDrawFunc(draw);
	RegisterDestroyFunc(destroy);
	RegisterUpdateFunc(update);

#ifdef WIN32
	WinLoop();
#endif

	return 0;
}
#include "Application.h"
#include "Lib.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "FileManager.h"
#include "Map.h"
#include "../Utilities/Utilities.h"


class Tester: ITouchListener
{
public:
	virtual void touchPerformed()
	{
	}
};

Application* Application::mpInstance = NULL;

Application* Application::getInstance()
{
	if(mpInstance == NULL)
	{
		mpInstance = new Application();
	}
	return mpInstance;
}

void Application::destroyInstance()
{
	SAFE_FREE(mpInstance);
}

void Application::init(unsigned int screenWidth, unsigned int screenHeight)
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;
#ifdef WIN32
	InitWindow(screenWidth, screenHeight);
	FileManager::getInstance()->init("../Resources");
#else
	FileManager::getInstance()->init("/sdcard/Download/Res");
#endif
	InputManager::getInstance()->init();
	ResourceManager::getInstance()->init("RM.txt");
	SceneManager::getInstance()->init("SM.txt");
	int n = 10;
	int** map = Map::getInstance()->genMap(n, n);
	SceneManager::getInstance()->genMapObject(map, n, n, SceneManager::getInstance()->mObjects[0]);
	FREE_2D_ARRAY(map, n);
}

void Application::render()
{
	SceneManager::getInstance()->render();
}

void Application::update()
{
	SceneManager::getInstance()->update();
	static int i = 0;
	static int pos = 10;
	if(InputManager::getInstance()->isKeyDown('C')){
		i = 1;
	}
	if(InputManager::getInstance()->isKeyUp('C') && i == 1)
	{
		i = 0;
		
		SceneManager::getInstance()->mMapObject->setCharacterPosition(pos--, 5);
	}
}

void Application::destroy()
{
	SceneManager::destroyInstance();
	ResourceManager::destroyInstance();
	InputManager::destroyInstance();
	FileManager::destroyInstance();
}

Application::~Application(void)
{
	destroy();
}
unsigned int Application::getScreenWidth()
{
	return mScreenWidth;
}
unsigned int Application::getScreenHeight()
{
	return mScreenHeight;
}
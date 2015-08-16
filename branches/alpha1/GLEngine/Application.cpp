#include "Application.h"
#include "Lib.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "FileManager.h"
#include "Map.h"
#include "../Utilities/Utilities.h"

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
	mStageManager = StageManager::getInstance();
	mStageManager->setStage(STAGE_PLAYING1);
}

void Application::render()
{
	mStageManager->render();
}

void Application::update()
{
	mStageManager->update();
}

void Application::destroy()
{
	FileManager::destroyInstance();
	InputManager::destroyInstance();
	StageManager::destroyInstance();
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
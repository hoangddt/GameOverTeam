#include "Application.h"
#include "Lib.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "FileManager.h"
#include "SoundManager.h"
#include "Map.h"
#include "../Utilities/Utilities.h"
#include "LogHeaderAndroid.h"
#include "Camera2D.h"

#if ANDROID
extern jobject globalActivity;
extern JavaVM* jVM;
#elif WIN32
extern HWND hWnd;
#endif

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
	glViewport(0, 0, screenWidth, screenHeight);
	// For emulator
	FileManager::getInstance()->init("/sdcard/Download/Resources");
	// for inside phone memory : A QUang - A Hai
	// FileManager::getInstance()->init("/storage/emulated/0/Download/Resources");
	// for a Hoang
	// FileManager::getInstance()->init("/storage/external_SD/Download/Resources");
#endif
	InputManager::getInstance()->init();
	SoundManager::getInstance()->init("Sounds");
	Camera2D::createInstance(screenWidth, screenHeight);
	mStageManager = StageManager::getInstance();
	mStageManager->setStage(STAGE_LOGO);
	printf("Exiting Application::init()\n");
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	Camera2D::destroyInstance();
	SoundManager::destroyInstance();
}

Application::~Application(void)
{
	destroy();
}

void Application::call_exit()
{
#if WIN32
	// exit and destroy application
	PostMessage(hWnd, WM_QUIT, 0, NULL);
#elif ANDROID
	//  com.gameover.androidport.MainActivity.exitApplication()
	JNIEnv* env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);

	jclass classMainActivity = env->GetObjectClass(globalActivity);

	jmethodID methodId = env->GetMethodID(classMainActivity, "exitApplication", "()V");
	// call exit application in Android
	env->CallVoidMethod(globalActivity, methodId);
#endif
}

unsigned int Application::getScreenWidth()
{
	return mScreenWidth;
}

unsigned int Application::getScreenHeight()
{
	return mScreenHeight;
}
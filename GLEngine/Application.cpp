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
#include "GameData.h"

/*
This code interact with Android, include:
*  Setting Resource Path
*  Call exit to close both Android and C++ app
*  Init SoundManager class and set Sound path
 */

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
#elif ANDROID
	/*-----  Setting viewport  ------*/
	glViewport(0, 0, screenWidth, screenHeight);
	/*==========  Get path from Java  ==========*/

	JNIEnv* env;
	jclass classMainActivity;
	jmethodID getPathId;

	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	classMainActivity = (jclass) env->FindClass("com/gameover/androidport/MainActivity");
	getPathId = env->GetStaticMethodID(classMainActivity, "getDataPath", "()Ljava/lang/String;");

	jstring resultJNIStr = (jstring) env->CallStaticObjectMethod(classMainActivity, getPathId);
/*
	JNIEnv* env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);

	jclass classMainActivity = env->GetObjectClass(globalActivity);

	jmethodID methodId = env->GetMethodID(classMainActivity, "getDataPath", "()Ljava/lang/String;");
	
	jstring resultJNIStr = (jstring) env->CallObjectMethod(classMainActivity, methodId);
*/
	jboolean iscopy;
	const char *resultCStr = env->GetStringUTFChars(resultJNIStr, &iscopy);
	
	if (NULL == resultCStr)
	{
		printf("Application::init:66 Can't convert jstring to C++ string \n");
	}
	printf("In C, the returned string is %s\n", resultCStr);

	/*-----  Setting resource path  ------*/
	FileManager::getInstance()->init(resultCStr);
    printf("Goi init thanh cong\n");
    env->ReleaseStringUTFChars(resultJNIStr, resultCStr);
	
#endif
	InputManager::getInstance()->init();
	GameData::getInstance()->init("GameData/GameData.txt");
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
void Application::pause()
{
	mStageManager->pause();
}

void Application::destroy()
{
	
	GameData::getInstance()->saveData();
	printf("GameData is saved\n");
	GameData::destroyInstance();

	InputManager::destroyInstance();
	StageManager::destroyInstance();
	Camera2D::destroyInstance();
	SoundManager::destroyInstance();
	FileManager::destroyInstance();
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
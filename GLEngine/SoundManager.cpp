#include "SoundManager.h"
#ifdef WIN32
#include <windows.h>
#pragma comment(lib, "winmm.lib")
#endif
#include <string.h>
#include "FileManager.h"
#include "Lib.h"
#include "LogHeaderAndroid.h"
#include "GameData.h"
/**
*
* This code interact with ANDROID app including:
*  - retrieve reference to Java Virtual Machine
*  - Retrieve JNI
*  - Get and call play method to play music
*  - Get and call stop method to stop music
*  - Set mute on/off on Android
*  - Init SoundManager class and set its path
*  - JNI docs about parameter: 
*    http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html#wp276
**/

#if ANDROID

JavaVM* jVM; // reference to curent java virtual machine

int JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("Just entered JNI_Onload");
	JNIEnv *env;
	jVM = vm;
	LOGI("JNI ONLOAD");
	if ( (vm)->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
	{
		LOGI("JNI NULL");
		return -1;
	}
	LOGI("Yeah! we got JNI");
	return JNI_VERSION_1_6;
}

void Android_play(const char* name, bool repeat)
{
	JNIEnv* env;
	jclass classSoundManager;
	jmethodID static_play;


	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	LOGI("Android_play get class");
	classSoundManager = (jclass) env->FindClass("com/gameover/androidport/SoundManager");

	// Get method id
	// 1st parameter: Ljava/lang/Sring  : string
	// 2nd parameter: Z                 : boolean
	// according to: http://docs.oracle.com/javase/1.5.0/docs/guide/jni/spec/types.html#wp276
 
	static_play = env->GetStaticMethodID(classSoundManager,
			"play", "(Ljava/lang/String;Z)V");

	
	jstring jmessage = env->NewStringUTF(name);
	LOGI("Android_play is calling Java method");
	env->CallStaticVoidMethod(classSoundManager, static_play, jmessage, repeat);
}

void Android_setMute(bool mute)
{
	JNIEnv* env;
	jclass classSoundManager;
	jmethodID static_setMute;

	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	classSoundManager = (jclass) env->FindClass("com/gameover/androidport/SoundManager");
	static_setMute = env->GetStaticMethodID(classSoundManager,
		"setMute", "(Z)V");

	env->CallStaticVoidMethod(classSoundManager, static_setMute, mute);
}

void Android_stop(const char* name)
{
	JNIEnv* env;
	jclass classSoundManager;
	jmethodID static_stop;

	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	classSoundManager = (jclass) env->FindClass("com/gameover/androidport/SoundManager");
	// Get method id
	static_stop = env->GetStaticMethodID(classSoundManager,
		"stop", "(Ljava/lang/String;)V");

	jstring jmessage = env->NewStringUTF(name);

	LOGI("Android_stop is calling Java method");
	env->CallStaticVoidMethod(classSoundManager, static_stop, jmessage);
}

#endif

SoundManager* SoundManager::mInstance = NULL;

SoundManager* SoundManager::getInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new SoundManager();
	}
	return mInstance;
}

void SoundManager::setMute(bool mute)
{
	mIsMute = mute;
#if ANDROID
	Android_setMute(mute);
#endif
}

void SoundManager::destroyInstance()
{
	SAFE_FREE(mInstance);
}

void SoundManager::init(const char* soundFolder)
{
	printf("SoundManager::init function\n");
	mRootPath = FileManager::getInstance()->combineWithRoot(soundFolder);
#if ANDROID
	/*==========  Get `init` method and call  ==========*/
	
	JNIEnv* env;
	jclass classSoundManager;
	jmethodID static_init;

	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	classSoundManager = (jclass) env->FindClass("com/gameover/androidport/SoundManager");
	// Get method id
	static_init = env->GetStaticMethodID(classSoundManager,
		"init", "(Ljava/lang/String;)V");

	jstring jSoundPath = env->NewStringUTF(mRootPath);

	LOGI("Android_init is calling Java method");
	env->CallStaticVoidMethod(classSoundManager, static_init, jSoundPath);
#endif
}

SoundManager::SoundManager()
{
	mRootPath = NULL;

	// Get sound status from GameData and set to SoundManager
	bool isMute = !GameData::getInstance()->getSoundSetting();
	mIsMute = isMute;
}

SoundManager::~SoundManager()
{
	SAFE_FREE(mRootPath);
}

void SoundManager::play(const char* id, bool repeat) {
	stop(id);
	// printf("Playing: %s repeat: %d\n", id, repeat);
	const char* fileName = FileManager::combine(mRootPath, id);

#ifdef WIN32
	char cmd[100];
	strcpy(cmd, "open ");
	strcat(cmd, fileName);
	strcat(cmd, " type mpegvideo alias ");
	strcat(cmd, id);
	mciSendStringA(cmd, NULL, 0, 0);

	strcpy(cmd, "play ");
	strcat(cmd, id);
	if(repeat)
	{
		strcat(cmd, " repeat");
	}
	mciSendStringA(cmd, NULL, 0, 0);
#else
	if (!mIsMute)
	{
		Android_play(id, repeat);
	}
#endif

	delete[] fileName;
}

void SoundManager::stop(const char* id)
{
#if WIN32
	char cmd[50];
	strcpy(cmd, "close ");
	strcat(cmd, id);
	mciSendStringA(cmd, NULL, 0, 0);
#else
	if (!mIsMute)
	{
		Android_stop(id);
	}
#endif
}

bool SoundManager::getMuteStatus()
{
	return mIsMute;
}
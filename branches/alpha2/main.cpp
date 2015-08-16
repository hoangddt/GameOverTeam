#if WIN32
#include "Application.h"
#include "../Utilities/Utilities.h"
#include "../lib/vld/vld.h"
#define LOGI printf
#elif ANDROID

#include <jni.h>
#include <android/log.h>

#include "OpenGLES2Header.h"

#include "Application.h"
#include "../Utilities/Matrix.h"

#define printf LOGI

#define  LOG_TAG    "MY_ANDROID_PORT_C"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

int SCREEN_W = 240;
int SCREEN_H = 320;

jobject globalActivity;

#endif

#if WIN32
void init()
{
	Application::getInstance()->init(480, 800);
}
#elif ANDROID
void init(int width, int height)
{
	LOGI("Enter init() function ");
	Application::getInstance()->init(width, height);
	LOGI("Exiting init() function ");
}
#endif
	

void update()
{
	Application::getInstance()->update();
}

void draw()
{
	// LOGI("Entered draw function \n");
	Application::getInstance()->render();
	// LOGI("Prepare to out draw()\n");
}

void destroy()
{
	Application::getInstance()->destroy();
	Application::getInstance()->destroyInstance();
	Application::destroyInstance();
}
#ifdef WIN32
int main()
{
	RegisterInitFunc(init);
	RegisterDrawFunc(draw);
	RegisterDestroyFunc(destroy);
	RegisterUpdateFunc(update);
	WinLoop();
	return 0;
}
#elif ANDROID


extern "C" {
    JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_step(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_destroy(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_onTouch(JNIEnv * env, jobject obj,  jint type, jint x, jint y);
	JNIEXPORT jstring JNICALL Java_com_gameover_androidport_SoundManager_nativeGetSoundPath(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_gameover_androidport_MainActivity_nativeInit(JNIEnv * env, jobject obj, jobject activity);
};

JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    init (width, height);
}

JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_step(JNIEnv * env, jobject obj)
{
	update();
    draw();
}

JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_destroy(JNIEnv * env, jobject obj)
{
	LOGI("Entered destroy function");
	destroy();
	LOGI("Prepare to exit destroy function");
}
JNIEXPORT void JNICALL Java_com_gameover_androidport_AndroidPortLib_onTouch(JNIEnv * env, jobject obj,  jint type, jint x, jint y)
{
	InputManager::onMouseEvent(type, x, y);
}

JNIEXPORT jstring JNICALL Java_com_gameover_androidport_SoundManager_nativeGetSoundPath(JNIEnv * env, jobject obj)
{
	const char* soundPath = FileManager::getInstance()->combineWithRoot("Sounds/");
	jstring result = env->NewStringUTF(soundPath);
	LOGI("The soundPath C++ pass to Java is: %s", soundPath);
	delete soundPath;
	return result;
}

JNIEXPORT void JNICALL Java_com_gameover_androidport_MainActivity_nativeInit(JNIEnv * env, jobject obj, jobject activity)
{
	globalActivity = env->NewGlobalRef(activity);
}
#endif

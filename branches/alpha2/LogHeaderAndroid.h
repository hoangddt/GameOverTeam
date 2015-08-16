#pragma once
#if ANDROID

#include <jni.h>
#include <android/log.h>

#define printf LOGI

#define  LOG_TAG    "MY_ANDROID_PORT_C"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif
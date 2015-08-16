LOCAL_PATH := $(call my-dir)

MY_ROOT_DIR := $(LOCAL_PATH)/../..
MY_PROJECT_DIR := $(MY_ROOT_DIR)/GLEngine
MY_UTILITIES_DIR := $(MY_ROOT_DIR)/Utilities
MY_RESOURCE_DIR := $(MY_ROOT_DIR)/Resources

include $(CLEAR_VARS)

MY_SRC_FILES := $(wildcard $(MY_PROJECT_DIR)/*.cpp)
MY_UTI_FILES := $(wildcard $(MY_UTILITIES_DIR)/*.cpp)

LOCAL_MODULE    := AndroidPort
LOCAL_CPP_FEATURES := exceptions

LOCAL_SRC_FILES := $(MY_SRC_FILES:$(LOCAL_PATH)/%=%) \
					$(MY_UTI_FILES:$(LOCAL_PATH)/%=%)

LOCAL_LDLIBS    := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)

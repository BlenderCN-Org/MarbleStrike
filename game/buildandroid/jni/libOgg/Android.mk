
LOCAL_PATH:= $(call my-dir)/../../..

include $(CLEAR_VARS)

LOCAL_MODULE := ogg_android

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/sdk/libogg-1.3.0/include \

LOCAL_CFLAGS := -Werror

LOCAL_SRC_FILES := \
    sdk/libogg-1.3.0/src/bitwise.c \
    sdk/libogg-1.3.0/src/framing.c \

include $(BUILD_STATIC_LIBRARY)
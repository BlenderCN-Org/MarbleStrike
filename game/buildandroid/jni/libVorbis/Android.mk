
LOCAL_PATH:= $(call my-dir)/../../..

include $(CLEAR_VARS)

LOCAL_MODULE := vorbis_android

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/sdk/libvorbis-1.3.3/include \
    $(LOCAL_PATH)/sdk/libvorbis-1.3.3/lib \
    $(LOCAL_PATH)/sdk/libogg-1.3.0/include \

LOCAL_CFLAGS := -Werror

LOCAL_SRC_FILES := \
    sdk/libvorbis-1.3.3/lib/analysis.c \
    sdk/libvorbis-1.3.3/lib/bitrate.c \
    sdk/libvorbis-1.3.3/lib/block.c \
    sdk/libvorbis-1.3.3/lib/codebook.c \
    sdk/libvorbis-1.3.3/lib/envelope.c \
    sdk/libvorbis-1.3.3/lib/floor0.c \
    sdk/libvorbis-1.3.3/lib/floor1.c \
    sdk/libvorbis-1.3.3/lib/info.c \
    sdk/libvorbis-1.3.3/lib/lookup.c \
    sdk/libvorbis-1.3.3/lib/lpc.c \
    sdk/libvorbis-1.3.3/lib/lsp.c \
    sdk/libvorbis-1.3.3/lib/mapping0.c \
    sdk/libvorbis-1.3.3/lib/mdct.c \
    sdk/libvorbis-1.3.3/lib/psy.c \
    sdk/libvorbis-1.3.3/lib/registry.c \
    sdk/libvorbis-1.3.3/lib/res0.c \
    sdk/libvorbis-1.3.3/lib/sharedbook.c \
    sdk/libvorbis-1.3.3/lib/smallft.c \
    sdk/libvorbis-1.3.3/lib/synthesis.c \
    sdk/libvorbis-1.3.3/lib/vorbisenc.c \
    sdk/libvorbis-1.3.3/lib/vorbisfile.c \
    sdk/libvorbis-1.3.3/lib/window.c \

include $(BUILD_STATIC_LIBRARY)
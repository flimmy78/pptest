LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

COMMON_PATH := ..
SRC_PATH := src/lib_json

LOCAL_MODULE := jsoncpp
LOCAL_CPPFLAGS := -fexceptions
LOCAL_LDLIBS := -llog
LOCAL_C_INCLUDES := $(COMMON_PATH)/include $(LOCAL_PATH)/include

LOCAL_SRC_FILES := \
	$(SRC_PATH)/json_reader.cpp \
	$(SRC_PATH)/json_value.cpp \
	$(SRC_PATH)/json_writer.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
	
include $(BUILD_STATIC_LIBRARY)

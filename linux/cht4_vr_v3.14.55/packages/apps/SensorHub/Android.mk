LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional


LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_PACKAGE_NAME := SensorHub
LOCAL_CERTIFICATE := platform

LOCAL_DEX_PREOPT:=false
LOCAL_PROGUARD_FLAGS := -include $(LOCAL_PATH)/proguard.flags
#LOCAL_PRIVILEGED_MODULE := true
include $(BUILD_PACKAGE)

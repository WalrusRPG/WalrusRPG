external_LOCAL_PATH := $(call whereami)

include $(wildcard $(external_LOCAL_PATH)/*/rules.mk)

INCLUDE += $(external_LOCAL_PATH)/tinystl/include

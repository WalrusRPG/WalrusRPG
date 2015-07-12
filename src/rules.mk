src_LOCAL_PATH := $(call whereami)

include $(wildcard $(src_LOCAL_PATH)/*/rules.mk)

SRCS_C += $(wildcard $(src_LOCAL_PATH)/*/*.c)
SRCS_CPP += $(wildcard $(src_LOCAL_PATH)/*/*.cpp)
INCLUDE += $(shell find $(src_LOCAL_PATH) -mindepth 1 -maxdepth 1 -type d)

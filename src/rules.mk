src_LOCAL_PATH := $(call whereami)

include $(wildcard $(src_LOCAL_PATH)/*/rules.mk)

SRCS_C += $(wildcard $(src_LOCAL_PATH)/*/*.c)
SRCS_CPP += $(wildcard $(src_LOCAL_PATH)/*/*.cpp)
INCLUDE += $(src_LOCAL_PATH)

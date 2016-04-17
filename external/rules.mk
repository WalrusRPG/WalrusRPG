external_LOCAL_PATH := $(call whereami)

include $(wildcard $(external_LOCAL_PATH)/*/rules.mk)

CPPFLAGS += -DLODEPNG_NO_COMPILE_CPP
INCLUDE_EXT += $(external_LOCAL_PATH)/lodepng
SRCS_CPP += $(external_LOCAL_PATH)/lodepng/lodepng.cpp

INCLUDE_EXT += $(external_LOCAL_PATH)/tinystl/include


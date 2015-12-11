external_LOCAL_PATH := $(call whereami)

INCLUDE_EXT += $(external_LOCAL_PATH)/lodepng

SRCS_CPP += $(external_LOCAL_PATH)/lodepng/lodepng.cpp

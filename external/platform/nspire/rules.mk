external_nspire_LOCAL_PATH := $(call whereami)

CPPFLAGS += -DLODEPNG_NO_COMPILE_CPP

INCLUDE_EXT += $(external_nspire_LOCAL_PATH)/lodepng

SRCS_CPP += $(external_nspire_LOCAL_PATH)/lodepng/lodepng.cpp


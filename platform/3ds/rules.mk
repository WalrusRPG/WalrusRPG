tds_LOCAL_PATH := $(call whereami)
include $(DEVKITARM)/3ds_rules

SRCS_C += $(wildcard $(tds_LOCAL_PATH)/platform/*.c)
SRCS_CPP += $(wildcard $(tds_LOCAL_PATH)/*.cpp)
INCLUDE += $(tds_LOCAL_PATH)/public
DEPS += lodepng

LIBDIRS	:= $(CTRULIB) $(CURDIR)/../libsf2d $(PORTLIBS)
INCLUDE_EXT += $(sfml_LOCAL_PATH)/public $(foreach dir,$(LIBDIRS),$(dir)/include)

ARCH = -march=armv6k -mtune=mpcore -mfloat-abi=hard

LIBS = -lm -lctru -lsf2d -lz
LDFLAGS += -specs=3dsx.specs -g -march=armv6k -mtune=mpcore -mfloat-abi=hard $(ARCH) -Wl,--gc-sections,-Map,$(notdir $*.map) $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

CFLAGS_COMMON += -DTARGET_3DS=1 -DARM11 -D_3DS -fno-rtti -fno-exceptions

APP_TITLE		:= WalrusRPG
APP_DESCRIPTION	:= Here we go, fellows.
APP_AUTHOR		:= Eiyeron

CC = arm-none-eabi-gcc
CPP = arm-none-eabi-g++

EXE = $(OUT)/$(NAME).3dsx

run: all
	3dslink $(EXE)

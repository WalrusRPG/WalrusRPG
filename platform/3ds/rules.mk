tds_LOCAL_PATH := $(call whereami)
include $(DEVKITARM)/3ds_rules

SRCS_C += $(wildcard $(tds_LOCAL_PATH)/platform/*.c)
SRCS_CPP += $(wildcard $(tds_LOCAL_PATH)/*.cpp)
INCLUDE += $(tds_LOCAL_PATH)/public
DEPS += lodepng

LIBDIRS	:= $(CTRULIB) $(CURDIR)/../libsf2d $(PORTLIBS)
INCLUDE_EXT += $(3ds_LOCAL_PATH)/public $(foreach dir,$(LIBDIRS),$(dir)/include)

ARCH = -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft -DARM11 -D_3DS

LIBS = -lcitro3d -lsf2d -lctru -lm -lz
LDFLAGS += -specs=3dsx.specs $(ARCH) -Wl,--gc-sections,-Map,$(notdir $*.map) $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

CFLAGS_COMMON += -DTARGET_3DS=1 $(ARCH) -fno-rtti -fno-exceptions

APP_TITLE		:= WalrusRPG
APP_DESCRIPTION	:= Here we go, fellows.
APP_AUTHOR		:= Eiyeron

CC = arm-none-eabi-gcc
CPP = arm-none-eabi-g++
LD = $(CPP)
EXE = $(OUT)/$(NAME).3dsx
SMDH = $(OUT)/$(NAME).smdh

RELEASE_SMDH := $(addprefix $(RELEASE_DIRECTORY)/, $(notdir $(SMDH)))

all: $(EXE) $(SMDH)

# SMDH file
$(RELEASE_DIRECTORY)/$(notdir $(SMDH)): $(SMDH)
	@echo "$^ => $(RELEASE_DIRECTORY)/$@"
	@cp -u "$^" "$(RELEASE_DIRECTORY)/$@"

release: $(RELEASE_SMDH)


run: all $(SMDH)
	3dslink $(EXE)

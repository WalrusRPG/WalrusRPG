psp2_LOCAL_PATH := $(call whereami)

SRCS_C += $(wildcard $(psp2_LOCAL_PATH)/platform/*.c)
SRCS_CPP += $(wildcard $(psp2_LOCAL_PATH)/*.cpp)
INCLUDE += $(psp2_LOCAL_PATH)/public
DEPS +=

LIBDIRS	:= $(VITASDK)/arm-vita-eabi/lib
INCLUDE_EXT += $(3ds_LOCAL_PATH)/public $(foreach dir,$(LIBDIRS),$(dir)/include)

ARCH =

LIBS = -lvita2d -lSceCtrl_stub -lSceDisplay_stub -lSceGxm_stub -lSceSysmodule_stub -lSceNet_stub -lSceNetCtl_stub -lpng16 -lz -lm -ldebugnet
LDFLAGS = -Wl,-q -o 

CFLAGS_COMMON += -DTARGET_PSP2=1 $(ARCH) -fno-rtti -fno-exceptions

APP_TITLE		:= WalrusRPG
APP_DESCRIPTION	:= Here we go, fellows.
APP_AUTHOR		:= Eiyeron

CC = arm-vita-eabi-gcc
CPP = arm-vita-eabi-g++
STRIP = arm-vita-eabi-strip

EXE = $(OUT)/$(NAME).vpk
SFO = $(OUT)/$(NAME).sfo
EBOOT = $(OUT)/eboot.bin
VELF = $(OUT)/$(NAME).velf

all: $(EXE)

$(EXE): $(EBOOT) $(SFO)
	vita-pack-vpk  -b $(EBOOT) -s $(SFO) $(EXE) $(foreach file,$(DATA_FILES),--add $(file)=$(file))

$(SFO):
	vita-mksfoex -s TITLE_ID=FEED00001 'WalrusRPG' $(SFO)

$(EBOOT): $(VELF)
	vita-make-fself $(VELF) $(EBOOT)

$(VELF): $(ELF)
	$(STRIP) -g $<
	vita-elf-create $< $@ || /bin/true


run: all 

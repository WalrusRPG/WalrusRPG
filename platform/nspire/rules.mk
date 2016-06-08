nspire_LOCAL_PATH := $(call whereami)

SRCS_C += $(wildcard $(nspire_LOCAL_PATH)/*.c)
SRCS_CPP += $(wildcard $(nspire_LOCAL_PATH)/*.cpp)
INCLUDE += $(nspire_LOCAL_PATH)/public
DEPS += lodepng

LDFLAGS +=  -fuse-ld=gold
CFLAGS_COMMON += -marm -DTARGET_NSPIRE=1 -DWRPG_EXCEPTIONS=1
YCM_EXTRA_CFLAGS := -m32 -I$(NDLESS_GIT)/ndless-sdk/include -I$(HOME)/.ndless/include

CC = nspire-gcc
CPP = nspire-g++

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)" --compress

EXE = $(OUT)/$(NAME).tns
DATA_FILE_SUFFIX=.tns

$(EXE): $(ELF)
	@mkdir -p $(dir $@)
	@echo "ZEHN: $@"
	@$(ZEHN) --input $(ELF) --output $(EXE) $(ZEHNFLAGS)

run: all
	@echo "TILP: $(EXE)"
	@tilp -ns $(EXE) > /dev/null

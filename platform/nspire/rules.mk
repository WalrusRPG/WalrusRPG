nspire_LOCAL_PATH := $(call whereami)

SRCS_C += $(wildcard $(nspire_LOCAL_PATH)/platform/*.c)
SRCS_CPP += $(wildcard $(nspire_LOCAL_PATH)/*.cpp)
INCLUDE += $(nspire_LOCAL_PATH)/public

CFLAGS_COMMON += -marm -DNSPIRE=1

CC = nspire-gcc
CPP = nspire-g++

ZEHN = genzehn
ZEHNFLAGS = --name "$(NAME)" --compress

PLATFORM = nspire
EXE = $(OUT)/$(NAME).tns

$(EXE): $(ELF)
	@mkdir -p $(dir $@)
	@echo "ZEHN: $@"
	@$(ZEHN) --input $(ELF) --output $(EXE) $(ZEHNFLAGS)

run: all
	@echo "TILP: $(EXE)"
	@tilp -ns $(EXE) > /dev/null

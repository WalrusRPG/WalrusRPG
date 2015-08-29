NAME = WalrusRPG

DEBUG = FALSE

CFLAGS_COMMON = -Wall -W $(addprefix -I,$(INCLUDE) $(INCLUDE_EXT)) -MMD -MP

ifeq ($(DEBUG),FALSE)
	CFLAGS_COMMON += -Ofast -flto
else
	CFLAGS_COMMON += -O0 -g
endif

CFLAGS = $(CFLAGS_COMMON) -std=gnu11

CPPFLAGS = $(CFLAGS_COMMON) -std=gnu++11

LDFLAGS = $(CFLAGS_COMMON) -Wl,--gc-sections

SRCS_C :=
SRCS_CPP :=
OBJS = $(BUILT_SRCS_C:%.c=%.o) $(BUILD_SRCS_CPP:%.cpp=%.o)
OBJS += $(addprefix $(OUT)/,$(SRCS_C:%.c=%.o) $(SRCS_CPP:%.cpp=%.o))
INCLUDE :=
INCLUDE_EXT :=
BUILT_SRCS :=
BUILT_SRCS_C :=
BUILT_SRCS_CPP :=

OUT = out
ELF = $(OUT)/$(NAME).elf

CLEAN_SPEC :=

# Figure out where we are.
define whereami
$(strip \
  $(eval LOCAL_MODULE_MAKEFILE := $$(lastword $$(MAKEFILE_LIST))) \
  $(if $(filter $(BUILD_SYSTEM)/% $(OUT_DIR)/%,$(LOCAL_MODULE_MAKEFILE)), \
    $(error my-dir must be called before including any other makefile.) \
   , \
    $(patsubst %/,%,$(dir $(LOCAL_MODULE_MAKEFILE))) \
   ) \
 )
endef

include config.mk
include rules.mk

config.mk:
	./mkconfig

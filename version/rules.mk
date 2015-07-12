version_LOCAL_PATH := $(call whereami)

SRCS_C += $(version_LOCAL_PATH)/version.c
INCLUDE += $(version_LOCAL_PATH)

CLEAN_SPEC += $(version_LOCAL_PATH)/version.c

$(version_LOCAL_PATH)/version.c: .FORCE
	@./$(version_LOCAL_PATH)/version.bash

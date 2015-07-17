version_LOCAL_PATH := $(call whereami)
version_SRC := $(OUT)/$(version_LOCAL_PATH)/version.c

BUILT_SRCS_C += $(version_SRC)
INCLUDE += $(version_LOCAL_PATH)

$(version_SRC): .FORCE
	@mkdir -p $(dir $@)
	@./$(version_LOCAL_PATH)/version.bash $@

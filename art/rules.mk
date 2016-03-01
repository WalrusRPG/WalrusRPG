art_LOCAL_PATH := $(call whereami)

art_SPRITES := $(wildcard $(art_LOCAL_PATH)/*.png)
art_SPR_DATA := $(addprefix $(OUT)/,$(art_SPRITES:%.png=%.cspr))
art_SPR_SRC := $(OUT)/$(art_LOCAL_PATH)/sprites.c
art_SPR_HDR := $(OUT)/$(art_LOCAL_PATH)/sprites.h

# BUILT_SRCS_C += $(art_SPR_SRC)
# INCLUDE += $(OUT)/$(art_LOCAL_PATH)
# BUILT_SRCS += $(art_SPR_HDR)

# $(OUT)/%.cspr: %.png
# 	@echo "SPRITE: $@"
# 	@mkdir -p $(dir $@)
# 	@ConvertImg --not-static --format n2dlib $< > $@

# $(art_SPR_SRC): $(art_SPR_DATA)
# 	@echo "Catting sprites into sprites.c"
# 	@rm -f $@
# 	@echo "#include <stdint.h>" > $@
# 	@cat $^ >> $@

# $(art_SPR_HDR): $(art_SPR_SRC)
# 	@./$(art_LOCAL_PATH)/header.bash $< $@

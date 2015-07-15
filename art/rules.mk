art_LOCAL_PATH := $(call whereami)

art_SPRITES := $(wildcard $(art_LOCAL_PATH)/*.png)
art_SPR_DATA := $(patsubst %.png,%.cspr,$(art_SPRITES))

SRCS_C += $(art_LOCAL_PATH)/sprites.c
INCLUDE += $(art_LOCAL_PATH)
BUILT_SRCS += $(art_LOCAL_PATH)/sprites.h

CLEAN_SPEC += $(art_SPR_DATA) $(art_LOCAL_PATH)/sprites.c $(art_LOCAL_PATH)/sprites.h

%.cspr: %.png
	@echo "Converting : $<"
	@ConvertImg --format n2dlib $< > $@

$(art_LOCAL_PATH)/sprites.c: $(art_SPR_DATA)
	@echo "Catting sprites into sprites.c"
	@rm -f $@
	@cat $^ | sed "s/^static uint16_t/unsigned short/" >> $@

$(art_LOCAL_PATH)/sprites.h: $(art_LOCAL_PATH)/sprites.c
	@echo "Making header sprites.h"
	@cat $< | grep "=" | sed -e "s/ =.*/;/" -e "s/^/extern /" > $@

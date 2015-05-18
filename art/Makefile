SPRITES = $(wildcard *.png)
SPR_DATA = $(patsubst %.png,%.cspr,$(SPRITES))

%.cspr: %.png
	@echo "Converting : $<"
	@ConvertImg --format n2dlib $< > $@

all: sprites.c sprites.h
	
sprites.c: | $(SPR_DATA)
	@echo "Catting sprites into sprites.c"
	@rm -f sprites.c
	@cat  $(SPR_DATA)  | sed "s/^static uint16_t/unsigned short/" >> sprites.c

sprites.h: sprites.c
	@echo "Making header sprites.h"
	@cat sprites.c | grep "=" | sed -e "s/ =.*/;/" -e "s/^/extern /" > sprites.h

clean:
	rm -f $(SPR_DATA) sprites.c sprites.h

SPRITES = $(wildcard *.png)
SPR_DATA = $(patsubst %.png,%.cspr,$(SPRITES))

%.cspr: %.png
	@echo "Converting : $<"
	@ConvertImg --format n2dlib $< > $@

all: $(SPR_DATA)
	@echo "Catting sprites into sprites.c"
	@rm -f sprites.c
	@cat  $(SPR_DATA)  | sed "s/^static uint16_t/unsigned short/" >> sprites.c
	@echo "Making header sprites.h"
	@makeheaders sprites.c

clean:
	rm -f $(SPR_DATA) sprites.c sprites.h

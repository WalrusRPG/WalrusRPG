SPRITES = $(wildcard *.png)

all:
	rm -f sprites.c
	for FILE in $(SPRITES); do ConvertImg --format n2dlib $$FILE | sed "s/^static uint16_t/unsigned short/" >> sprites.c; done

clean:
	rm -f sprites.c sprites.h


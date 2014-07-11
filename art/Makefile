SPRITES = $(wildcard *.png)

all:
	rm -f sprites.c
	for FILE in $(SPRITES); do exec ./sprite.py $$FILE >> sprites.c; done

clean:
	rm -f sprites.c sprites.h


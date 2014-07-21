SPRITES = $(wildcard *.png)

all:
	rm -f sprites.c
	echo "#include <os.h>" > sprites.c
	for FILE in $(SPRITES); do ConvertImg --format n2dlib $$FILE >> sprites.c; done

clean:
	rm -f sprites.c sprites.h


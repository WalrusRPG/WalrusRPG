#ifndef SRC_MAP_H
#define SRC_MAP_H

typedef struct
{
	unsigned w;
	unsigned h;
	unsigned *layer0;
	unsigned *layer1;
} Map;

void map_draw(unsigned x, unsigned y, const Map map);
unsigned map_collide(unsigned x, unsigned y, const Map map);
void map_walk(unsigned x, unsigned y, Map map);

#endif

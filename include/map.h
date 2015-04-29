#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

typedef struct Map Map_t;
struct Map
{
	unsigned w;
	unsigned h;
	unsigned *layer0;
	unsigned *layer1;
};

void map_draw(unsigned x, unsigned y, const Map_t *map);
unsigned map_collide(unsigned x, unsigned y, const Map_t *map);
void map_loop(unsigned x, unsigned y, Map_t *map);

#endif

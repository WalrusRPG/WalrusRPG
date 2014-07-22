#include <os.h>
#include "n2DLib/n2DLib.h"
#include "map.h"

#if INTERFACE
typedef struct
{
	unsigned w;
	unsigned h;
	unsigned *ground;
} Map;
#endif

void map_draw(unsigned x, unsigned y, const Map *map)
{
	unsigned tile_x = x / 24;
	unsigned tile_y = y / 24;
	unsigned offset_x = x % 24;
	unsigned offset_y = y % 24;

	unsigned i, j, tile;
	Rect sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	for (i = 0; i < 15; i++)
	for (j = 0; j < 11; j++)
	{
		tile = map->ground[(tile_x + i) + (tile_y + j) * map->w];
		sprite.x = tile * 24;
		drawSpritePart(tiles, (i * 24) - offset_x, (j * 24) - offset_y, &sprite);
	}
}


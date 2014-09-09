#include <os.h>
#include "n2DLib/n2DLib.h"
#include "map.h"

#if INTERFACE
typedef struct
{
	unsigned w;
	unsigned h;
	unsigned *layer0;
	unsigned *layer1;
	unsigned *layer2;
} Map;
#endif

void map_draw(unsigned x, unsigned y, const Map map)
{
	unsigned tile_x = x / 24;
	unsigned tile_y = y / 24;
	unsigned offset_x = x % 24;
	unsigned offset_y = y % 24;

	unsigned i, j, tile_offset;
	Rect sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	for (i = 0; i < 15; i++)
	for (j = 0; j < 11; j++)
	{
		tile_offset = (tile_x + i) + (tile_y + j) * map.w;

		sprite.x = map.layer0[tile_offset] * 24;
		if (sprite.x) drawSpritePart(tiles, (i * 24) - offset_x, (j * 24) - offset_y, &sprite);

		sprite.x = map.layer1[tile_offset] * 24;
		if (sprite.x) drawSpritePart(tiles, (i * 24) - offset_x, (j * 24) - offset_y, &sprite);

		sprite.x = map.layer2[tile_offset] * 24;
		if (sprite.x) drawSpritePart(tiles, (i * 24) - offset_x, (j * 24) - offset_y, &sprite);
	}
}


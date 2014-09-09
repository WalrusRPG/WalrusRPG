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
} Map;
#endif

void map_draw(unsigned x, unsigned y, const Map map)
{
	unsigned offset_x = x % 24 * -1;
	unsigned offset_y = y % 24 * -1;

	unsigned i, j, tile_offset;
	Rect sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	tile_offset = x / 24 + y / 24 * map.w;

	for (j = 0; j < 11; j++)
	{
		for (i = 0; i < 15; i++)
		{
			sprite.x = map.layer0[tile_offset] * 24;
			drawSpritePart(tiles, offset_x, offset_y, &sprite);

			sprite.x = map.layer1[tile_offset] * 24;
			drawSpritePart(tiles, offset_x, offset_y, &sprite);

			tile_offset++;
			offset_x += 24;
		}

		tile_offset += map.w - 15;
		offset_x -= 24 * 15;
		offset_y += 24;
	}
}


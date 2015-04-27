#include <os.h>
#include <timers.h>
#include <graphics.h>
#include <sprites.h>
#include <map.h>

static void map_walk_speed_load(unsigned time);
static unsigned map_walk_speed_read(unsigned time, unsigned div);

void map_draw(unsigned x, unsigned y, const Map_t map)
{
	x += 20;
	y += 12;

	unsigned offset_x = x % 24 * -1;
	unsigned offset_y = y % 24 * -1;

	unsigned i, j;
	Rect_t sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	for (j = 0; j < 11; j++)
	for (i = 0; i < 15; i++)
	{
		sprite.x = map.layer0[(x / 24 - 7 + i) + (y / 24 - 5 + j) * map.w] * 24;
		draw_sprite_sheet(tiles, offset_x + i * 24, offset_y + j * 24, &sprite);
	}
}

unsigned map_collide(unsigned x, unsigned y, const Map_t map)
{
	(void) x;
	(void) y;
	(void) map;
	return 0;
}

static void map_walk_speed_load(unsigned time)
{
	timer_load(0, time);
	while (timer_read(0) != time);
}

static unsigned map_walk_speed_read(unsigned time, unsigned div)
{
	return (time - timer_read(0)) / div;
}

void map_walk(unsigned x, unsigned y, Map_t map)
{
	unsigned i, walk_time, walk_div;

	while (!isKeyPressed(KEY_NSPIRE_ESC))
	{
		if (isKeyPressed(KEY_NSPIRE_VAR))
		{
			walk_time = 6554;
			walk_div = 273;
		}
		else
		{
			walk_time = 13107;
			walk_div = 546;
		}

		if (isKeyPressed(KEY_NSPIRE_5) && !map_collide(x, y + 1, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 0; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24, y * 24 + i, map);
					buffer_swap();
				}
			}

			y++;
		}

		else if (isKeyPressed(KEY_NSPIRE_8) && !map_collide(x, y - 1, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 0; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24, y * 24 - i, map);
					buffer_swap();
				}
			}

			y--;
		}

		else if (isKeyPressed(KEY_NSPIRE_4) && !map_collide(x - 1, y, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 0; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24 - i, y * 24, map);
					buffer_swap();
				}
			}

			x--;
		}

		else if (isKeyPressed(KEY_NSPIRE_6) && !map_collide(x + 1, y, map))
		{
			map_walk_speed_load(walk_time);

			for (i = 0; i < 24; i++)
			{
				if (i >= map_walk_speed_read(walk_time, walk_div))
				{
					while (i > map_walk_speed_read(walk_time, walk_div));
					map_draw(x * 24 + i, y * 24, map);
					buffer_swap();
				}
			}

			x++;
		}

		else
		{
			map_draw(x * 24, y * 24, map);
			buffer_swap();
		}
	}
}


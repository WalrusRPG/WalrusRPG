#include <os.h>
#include <timers.h>
#include <graphics.h>
#include <sprites.h>
#include <map.h>

void map_draw(unsigned x, unsigned y, const Map_t *map)
{
	unsigned offset_x = x % 24 * -1;
	unsigned offset_y = y % 24 * -1;

	Rect_t sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	for (unsigned j = 0; j < 11; j++)
	for (unsigned i = 0; i < 15; i++)
	{
		sprite.x = map->layer0[(x / 24 + i) + (y / 24 + j) * map->w] * 24;
		draw_sprite_sheet(tiles, offset_x + i * 24, offset_y + j * 24, &sprite);
	}
}

unsigned map_collide(unsigned x, unsigned y, const Map_t *map)
{
	(void) x;
	(void) y;
	(void) map;
	return 0;
}

void map_loop(unsigned x, unsigned y, Map_t *map)
{
	timer_mode(0, 0b0000010); // Free-running, no interrupts, divider = 1, 32 bit, wrapping
	timer_load(0, 0);
	unsigned loop_time = 546; // 32768Hz/60ups
	unsigned loop_next = -loop_time;

	unsigned keep_running = 1;
	while (keep_running)
	{
		if (isKeyPressed(KEY_NSPIRE_ESC)) keep_running = 0;
		if (isKeyPressed(KEY_NSPIRE_5)) y++;
		if (isKeyPressed(KEY_NSPIRE_8)) y--;
		if (isKeyPressed(KEY_NSPIRE_6)) x++;
		if (isKeyPressed(KEY_NSPIRE_4)) x--;

		// Frameskip
		if (timer_read(0) > loop_next)
		{
			map_draw(x, y, map);
			buffer_swap();
		}

		// Frame limiting
		while (timer_read(0) > loop_next);
		loop_next -= loop_time;
	}
}


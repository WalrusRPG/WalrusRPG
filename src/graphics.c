#include <os.h>
#include "graphics.h"

#if INTERFACE
typedef struct
{
	int x, y, w, h;
} Rect;
#endif

#define LCD_CONTROLLER 0xC0000000
volatile unsigned *lcd_base = (unsigned *) (LCD_CONTROLLER + 0x10);

#define BUFFER_SIZE 320 * 240 * 2
unsigned short *buffer_front = NULL, *buffer_back = NULL, *buffer_os;
unsigned buffer_front_offset, buffer_back_offset;

/*
 * Buffer management
 */

void buffer_allocate()
{
	buffer_front = (unsigned short *) malloc(BUFFER_SIZE + 8);
	buffer_back = (unsigned short *) malloc(BUFFER_SIZE + 8);

	if (buffer_front == NULL || buffer_back == NULL)
	{
		free(buffer_front);
		free(buffer_back);
		exit(0);
	}

	buffer_front_offset = 8 - ((unsigned) buffer_front & 0x7);
	buffer_back_offset = 8 - ((unsigned) buffer_back & 0x7);
	buffer_front = (unsigned short *) ((unsigned) buffer_front + buffer_front_offset);
	buffer_back = (unsigned short *) ((unsigned) buffer_back + buffer_back_offset);

	buffer_os = (unsigned short *) *lcd_base;
	*lcd_base = (unsigned) buffer_front;
}

void buffer_free()
{
	free((unsigned short *) ((unsigned) buffer_front - buffer_front_offset));
	free((unsigned short *) ((unsigned) buffer_back - buffer_back_offset));

	*lcd_base = (unsigned) buffer_os;
}

void buffer_swap()
{
	unsigned short *buffer_front_tmp = buffer_front;
	unsigned buffer_front_offset_tmp = buffer_front_offset;
	buffer_front = buffer_back;
	buffer_front_offset = buffer_back_offset;
	buffer_back = buffer_front_tmp;
	buffer_back_offset = buffer_front_offset_tmp;

	*lcd_base = (unsigned) buffer_front;
}

void buffer_copy()
{
	memcpy(buffer_front, buffer_back, BUFFER_SIZE);
}

void buffer_fill(unsigned color)
{
	unsigned *buffer_back_32 = (unsigned *) buffer_back;
	int i;
	color += color << 16;
	for (i = 0; i < (BUFFER_SIZE / 4); i++)
		buffer_back_32[i] = color;
}


/*
 * Drawing
 */

void draw_pixel(unsigned x, unsigned y, unsigned short color)
{
	if (x < 320 && y < 240)
		buffer_back[x + (y * 320)] = color;
}

void draw_sprite_sheet(const unsigned short *sheet, int x, int y, const Rect* window)
{
	unsigned short color;
	int w = window->w + x;
	int h = window->h + y;
	int i, j, k, l;

	for (j = y, l = window->y; j < h; j++, l++)
	for (i = x, k = window->x; i < w; i++, k++)
	{
		color = sprite_pixel_get(sheet, k, l);
		if (color != sheet[2])
			draw_pixel(i, j, color);
	}
}


/*
 * Sprite manipulation
 */

unsigned short sprite_pixel_get(const unsigned short *sprite, unsigned x, unsigned y)
{
	if (x < sprite[0] && y < sprite[1])
		return sprite[x + (y * sprite[0]) + 3];
	else
		return sprite[2];
}


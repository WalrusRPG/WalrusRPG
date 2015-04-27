#include <os.h>
#include "graphics.h"

#define LCD_CONTROLLER 0xC0000000
volatile unsigned *lcd_base = (unsigned *) (LCD_CONTROLLER + 0x10);
volatile unsigned *lcd_ris = (unsigned *) (LCD_CONTROLLER + 0x20);
volatile unsigned *lcd_icr = (unsigned *) (LCD_CONTROLLER + 0x28);
volatile unsigned *lcd_control = (unsigned *) (LCD_CONTROLLER + 0x18);
unsigned lcd_control_bkp;

#define BUFFER_SIZE 320 * 240 * 2
unsigned short *buffer_front = NULL, *buffer_back = NULL, *buffer_os;

/*
 * Buffer management
 */

void buffer_allocate()
{
	buffer_front = (unsigned short *) malloc(BUFFER_SIZE);
	buffer_back = (unsigned short *) malloc(BUFFER_SIZE);

	if (buffer_front == NULL || buffer_back == NULL)
	{
		free(buffer_front);
		free(buffer_back);
		exit(0);
	}

	buffer_os = (unsigned short *) *lcd_base;
	*lcd_base = (unsigned) buffer_front;

	// Set up the controller in order to use vsync signals
	lcd_control_bkp = *lcd_control;
	*lcd_control &= ~(0b11 << 12);
	*lcd_control |= 0b11 << 12;
}

void buffer_free()
{
	free(buffer_front);
	free(buffer_back);

	*lcd_base = (unsigned) buffer_os;

	*lcd_control = lcd_control_bkp;
}

void buffer_swap()
{
	unsigned short *buffer_front_tmp = buffer_front;
	buffer_front = buffer_back;
	buffer_back = buffer_front_tmp;

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
 * Misc LCD functions
 */

void lcd_vsync()
{
	*lcd_icr = 1 << 3;
	while (!(*lcd_ris & (1 << 3)));
}


/*
 * Drawing
 */

void draw_pixel(unsigned x, unsigned y, unsigned short color)
{
	if (x < 320 && y < 240)
		buffer_back[x + (y * 320)] = color;
}

void draw_sprite_sheet(const unsigned short *sheet, int x, int y, const Rect *window)
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


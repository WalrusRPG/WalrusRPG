#include <os.h>
#include "Graphics.h"
#include "utility/misc.h"

#define GRAPHICS WalrusRPG::Graphics

#define LCD_CONTROLLER 0xC0000000
volatile unsigned *lcd_base = (unsigned *) (LCD_CONTROLLER + 0x10);
volatile unsigned *lcd_ris = (unsigned *) (LCD_CONTROLLER + 0x20);
volatile unsigned *lcd_icr = (unsigned *) (LCD_CONTROLLER + 0x28);
volatile unsigned *lcd_control = (unsigned *) (LCD_CONTROLLER + 0x18);
unsigned lcd_control_bkp;
volatile unsigned *lcd_imsc = (unsigned *) (LCD_CONTROLLER + 0x1C);
unsigned lcd_imsc_bkp;

#define BUFFER_SIZE 320 * 240 * 2
unsigned short *buffer_screen = NULL, *buffer_render = NULL, *buffer_ready = NULL,
               *buffer_os;
bool buffer_swap_ready;

/*
 * Buffer management
 */

void GRAPHICS::buffer_allocate()
{
    buffer_screen = (unsigned short *) malloc(BUFFER_SIZE);
    buffer_render = (unsigned short *) malloc(BUFFER_SIZE);
    buffer_ready = (unsigned short *) malloc(BUFFER_SIZE);

    if (buffer_screen == NULL || buffer_render == NULL || buffer_ready == NULL)
    {
        free(buffer_screen);
        free(buffer_render);
        free(buffer_ready);
        exit(0);
    }

    buffer_os = (unsigned short *) *lcd_base;
    *lcd_base = (unsigned) buffer_screen;
    buffer_swap_ready = false;

    // Set up the controller in order to use vsync signals
    lcd_control_bkp = *lcd_control;
    *lcd_control &= ~(0b11 << 12);
    *lcd_control |= 0b11 << 12;
    lcd_imsc_bkp = *lcd_imsc;
    *lcd_imsc = 1 << 3;
}

void GRAPHICS::buffer_free()
{
    free(buffer_screen);
    free(buffer_render);
    free(buffer_ready);

    *lcd_base = (unsigned) buffer_os;

    *lcd_control = lcd_control_bkp;
    *lcd_imsc = lcd_imsc_bkp;
}

void GRAPHICS::buffer_swap_screen()
{
    if (buffer_swap_ready)
    {
        unsigned short *buffer_screen_tmp = buffer_screen;
        buffer_screen = buffer_ready;
        buffer_ready = buffer_screen_tmp;

        *lcd_base = (unsigned) buffer_screen;
        buffer_swap_ready = false;
    }
}

void GRAPHICS::buffer_swap_render()
{
    unsigned short *buffer_ready_tmp = buffer_ready;
    buffer_ready = buffer_render;
    buffer_render = buffer_ready_tmp;
    buffer_swap_ready = true;
}

void GRAPHICS::buffer_fill(unsigned color)
{
    unsigned *buffer_render_32 = (unsigned *) buffer_render;
    color |= color << 16; // To avoid stupid overflows
    for (unsigned i = 0; i < (BUFFER_SIZE / 4); i++)
        buffer_render_32[i] = color;
}


/*
 * Misc LCD functions
 */

void GRAPHICS::lcd_vsync()
{
    *lcd_icr = 1 << 3;
    while (!(*lcd_ris & (1 << 3)))
        ;
}

void GRAPHICS::vsync_isr()
{
    buffer_swap_screen();
    *lcd_icr = 1 << 3;
}


/*
 * Drawing
 */

void GRAPHICS::draw_pixel(unsigned x, unsigned y, unsigned short color)
{
    buffer_render[x + (y * 320)] = color;
}

void GRAPHICS::draw_sprite_sheet(const unsigned short *sheet, int x, int y,
                                 const WalrusRPG::Utils::Rect &window)
{
    unsigned short color;
    int w = min(window.width + x, 320);
    int h = min(window.height + y, 240);

    for (int j = max(y, 0), l = window.y - min(y, 0); j < h; j++, l++)
    {
        for (int i = max(x, 0), k = window.x - min(x, 0); i < w; i++, k++)
        {
            color = sprite_pixel_get(sheet, k, l);
            if (color != sheet[2])
                draw_pixel(i, j, color);
        }
    }
}


/*
 * Sprite manipulation
 */

unsigned short GRAPHICS::sprite_pixel_get(const unsigned short *sprite, unsigned x,
                                          unsigned y)
{
    if (x < sprite[0] && y < sprite[1])
        return sprite[x + (y * sprite[0]) + 3];
    else
        return sprite[2];
}

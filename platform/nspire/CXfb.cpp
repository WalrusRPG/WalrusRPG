#include "CXfb.h"
#include "mutex.h"
#include "utility/minmax.h"
#include "utility/misc.h"
#include <cstdlib>
#include <cstring>

#define GRAPHICS Nspire::CXfb

#define LCD_CONTROLLER 0xC0000000
volatile uint32_t *lcd_base = (uint32_t *) (LCD_CONTROLLER + 0x10);
volatile uint32_t *lcd_ris = (uint32_t *) (LCD_CONTROLLER + 0x20);
volatile uint32_t *lcd_icr = (uint32_t *) (LCD_CONTROLLER + 0x28);
volatile uint32_t *lcd_control = (uint32_t *) (LCD_CONTROLLER + 0x18);
uint32_t lcd_control_bkp;
volatile uint32_t *lcd_imsc = (uint32_t *) (LCD_CONTROLLER + 0x1C);
uint32_t lcd_imsc_bkp;

#define BUFFER_SIZE 320 * 240 * 2
uint16_t *buffer_screen = NULL, *buffer_render = NULL, *buffer_ready = NULL, *buffer_os;
unsigned buffer_swap_lock;
bool buffer_swap_ready;

/*
 * Buffer management
 */

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void GRAPHICS::buffer_allocate()
{
    buffer_screen = (uint16_t *) malloc(BUFFER_SIZE);
    buffer_render = (uint16_t *) malloc(BUFFER_SIZE);
    buffer_ready = (uint16_t *) malloc(BUFFER_SIZE);

    if (buffer_screen == NULL || buffer_render == NULL || buffer_ready == NULL)
    {
        free(buffer_screen);
        free(buffer_render);
        free(buffer_ready);
        exit(0);
    }

    memset(buffer_screen, 0, BUFFER_SIZE);

    buffer_os = (uint16_t *) *lcd_base;
    *lcd_base = (uint32_t) buffer_screen;
    buffer_swap_lock = 0;
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

    *lcd_base = (uint32_t) buffer_os;

    *lcd_control = lcd_control_bkp;
    *lcd_imsc = lcd_imsc_bkp;
}

void GRAPHICS::buffer_swap_screen()
{
    if (mutex_lock(&buffer_swap_lock))
        return;

    if (buffer_swap_ready)
    {
        uint16_t *buffer_screen_tmp = buffer_screen;
        buffer_screen = buffer_ready;
        buffer_ready = buffer_screen_tmp;

        *lcd_base = (uint32_t) buffer_screen;
        buffer_swap_ready = false;
    }

    mutex_unlock(&buffer_swap_lock);
}

void GRAPHICS::buffer_swap_render()
{
    spin_lock(&buffer_swap_lock);

    uint16_t *buffer_ready_tmp = buffer_ready;
    buffer_ready = buffer_render;
    buffer_render = buffer_ready_tmp;
    buffer_swap_ready = true;

    mutex_unlock(&buffer_swap_lock);
}

void GRAPHICS::buffer_fill(uint16_t color)
{
    uint32_t *buffer_render_32 = (uint32_t *) buffer_render;
    uint32_t color_32 = color << 16 | color; // To avoid stupid overflows
    for (uint32_t i = 0; i < (BUFFER_SIZE / 4); i++)
        buffer_render_32[i] = color_32;
}


/*
 * Misc LCD functions
 */

void GRAPHICS::vsync_isr()
{
    buffer_swap_screen();
    *lcd_icr = 1 << 3;
}


/*
 * Drawing
 */

void GRAPHICS::draw_pixel(int x, int y, uint16_t color)
{
    buffer_render[x + (y * 320)] = color;
}

void GRAPHICS::draw_pixel_tint(int x, int y, uint16_t color, uint16_t tint)
{
    int r = ((color >> 11) * (tint >> 11)) >> 5;
    int g = (((color >> 5) & 0b111111) * ((tint >> 5) & 0b111111)) >> 6;
    int b = ((color & 0b11111) * (tint & 0b11111)) >> 5;
    buffer_render[x + (y * 320)] = (r << 11) | (g << 5) | b;
}

void GRAPHICS::draw_sprite_sheet(const uint16_t *sheet, int x, int y,
                                 const WalrusRPG::Utils::Rect &window)
{
    uint16_t color;
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

void GRAPHICS::draw_sprite_sheet_tint(const uint16_t *sheet, int x, int y,
                                      const WalrusRPG::Utils::Rect &window, uint16_t tint)
{
    uint16_t color;
    int w = min(window.width + x, 320);
    int h = min(window.height + y, 240);

    for (int j = max(y, 0), l = window.y - min(y, 0); j < h; j++, l++)
    {
        for (int i = max(x, 0), k = window.x - min(x, 0); i < w; i++, k++)
        {
            color = sprite_pixel_get(sheet, k, l);
            if (color != sheet[2])
                draw_pixel_tint(i, j, color, tint);
        }
    }
}


/*
 * Sprite manipulation
 */

uint16_t GRAPHICS::sprite_pixel_get(const uint16_t *sprite, uint32_t x, uint32_t y)
{
    if (x < sprite[0] && y < sprite[1])
        return sprite[x + (y * sprite[0]) + 3];
    else
        return sprite[2];
}

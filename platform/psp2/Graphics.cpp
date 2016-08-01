#include "Graphics.h"
#include "Logger.h"
#include "render/Pixel.h"
#include "utility/Rect.h"
#include "utility/minmax.h"
#include "utility/misc.h"
#include <vita2d.h>

using namespace WalrusRPG; /*::Graphics*/
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::Utils::Rect;


namespace
{
    inline uint32_t pixel2u32(const Pixel &pix)
    {
        return RGBA8(pix.r << 3, pix.g << 2, pix.b << 3, 0xFF);
    }
    constexpr int OFFSET_X = 0;
    constexpr int OFFSET_Y = 0;
    void set_scissor()
    {
        vita2d_set_region_clip(SCE_GXM_REGION_CLIP_OUTSIDE, 0, 0, 320, 240);
    }
}

void Graphics::init()
{
    // Logger::log("Graphics init");
    vita2d_init();
    vita2d_set_clear_color(RGBA8(0x0, 0, 0, 0xFF));
    vita2d_set_vblank_wait(1);
    vita2d_set_region_clip(SCE_GXM_REGION_CLIP_OUTSIDE, 0, 0, 320, 240);
}

void Graphics::deinit()
{
    Logger::log("Graphics deinit");
    vita2d_fini();
}

void Graphics::frame_begin()
{
    vita2d_start_drawing();
    vita2d_clear_screen();
}

void Graphics::frame_end()
{
    vita2d_end_drawing();
    vita2d_swap_buffers();
}

void Graphics::put_sprite(const Texture &sheet, int x, int y, const Rect &window)
{
    vita2d_draw_texture_part_scale(sheet.data, x * 2 + OFFSET_X, y * 2 + OFFSET_Y,
                                   window.x, window.y, window.width, window.height, 2, 2);
}

void Graphics::put_sprite_tint(const Texture &sheet, int x, int y, const Rect &window,
                               const Pixel &color)
{
    vita2d_draw_texture_tint_part_scale(sheet.data, x * 2 + OFFSET_X, y * 2 + OFFSET_Y,
                                        window.x, window.y, window.width, window.height,
                                        2, 2, pixel2u32(color));
}

void Graphics::put_sprite_clipping(const Texture &sheet, int x, int y,
                                   const Rect &sprite_window, const Rect &clipping_window)
{
    vita2d_set_region_clip(SCE_GXM_REGION_CLIP_OUTSIDE, clipping_window.x + OFFSET_X,
                           clipping_window.y + OFFSET_Y, clipping_window.width,
                           clipping_window.height);
    vita2d_draw_texture_part_scale(sheet.data, x * 2, y * 2, sprite_window.x,
                                   sprite_window.y, sprite_window.width,
                                   sprite_window.height, 2, 2);
    set_scissor();
}


void Graphics::fill(const Pixel &color)
{
    vita2d_draw_rectangle(0, 0, 320, 240, pixel2u32(color));
}

void Graphics::put_pixel(uint16_t x, uint16_t y, const Pixel &color)
{
    vita2d_draw_pixel(x, y, pixel2u32(color));
}

void Graphics::put_horizontal_line(uint16_t x, uint16_t x2, uint16_t y,
                                   const Pixel &color)
{
    vita2d_draw_line(x * 2, y * 2, x2 * 2, y * 2, color);
    return;
    // Because sf2dlib has issues with lines, let's port Nspire's functions.
    if (x > x2)
    {
        uint16_t temp = x;
        x = x2;
        x2 = temp;
    }
    for (; x <= x2; x++)
    {
        put_pixel(x + OFFSET_X, y + OFFSET_Y, color);
    }
}

void Graphics::put_vertical_line(uint16_t x, uint16_t y, uint16_t y2, const Pixel &color)
{
    vita2d_draw_line(x * 2, y * 2, x * 2, y2 * 2, color);
    return;
    // Because sf2dlib has issues with lines, let's port Nspire's functions.
    if (y > y2)
    {
        uint16_t temp = y;
        y = y2;
        y2 = temp;
    }
    for (; y <= y2; y++)
    {
        put_pixel(x + OFFSET_X, y + OFFSET_Y, color);
    }
}

void Graphics::put_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2,
                        const Pixel &color)
{
    vita2d_draw_line(x * 2, y - 2, x2 * 2, y2 * 2, color);
    return;

    // Because sf2dlib has issues with lines, let's port Nspire's functions.
    if (x == x2)
    {
        put_vertical_line(x + OFFSET_X, y + OFFSET_Y, y2 + OFFSET_Y, color);
        return;
    }
    else if (y == y2)
    {
        put_horizontal_line(x + OFFSET_X, x2 + OFFSET_X, y + OFFSET_Y, color);
        return;
    }
    int dx = abs(x - x2), sx = x < x2 ? 1 : -1;
    int dy = abs(y - y2), sy = y < y2 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;

    while (true)
    {
        put_pixel(x + OFFSET_X, y + OFFSET_Y, color);
        if (x == x2 && y == y2)
            break;
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y += sy;
        }
    }
}

void Graphics::put_rectangle(const Rect &rect, const Pixel &color)
{
    vita2d_draw_rectangle(rect.x * 2 + OFFSET_X, rect.y * 2 + OFFSET_Y, rect.width * 2,
                          rect.height * 2, pixel2u32(color));
}

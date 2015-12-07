#include "Graphics.h"
#include "CXfb.h"
#include "stdio.h"
#include "utility/misc.h"

using namespace Nspire;
using namespace WalrusRPG;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::Utils::Rect;

void Graphics::init()
{
    CXfb::buffer_allocate();
}

void Graphics::deinit()
{
    CXfb::buffer_free();
}

void Graphics::frame_begin()
{
}

void Graphics::frame_end()
{
    CXfb::buffer_swap_render();
}

void Graphics::put_sprite(const Texture &sheet, int x, int y,
                          const Rect &window)
{
    CXfb::draw_sprite_sheet(sheet.data, x, y, window);
}

void Graphics::put_sprite_tint(const Texture &sheet, int x, int y,
                               const Rect &window,
                               const Pixel &color)
{
    CXfb::draw_sprite_sheet_tint(sheet.data, x, y, window, color.value);
}

void Graphics::fill(const Pixel &color)
{
    CXfb::buffer_fill(color);
}

void Graphics::put_pixel(uint16_t x, uint16_t y, const Pixel &color)

{
    CXfb::draw_pixel(x, y, color.value);
}

void Graphics::put_horizontal_line(uint16_t x, uint16_t x2, uint16_t y, const Pixel &color)
{
    if(x > x2)
    {
        uint16_t temp = x;
        x = x2;
        x2 = temp;
    }
    for (; x < x2; x++) {
        CXfb::draw_pixel(x, y, color);
    }
}

void Graphics::put_vertical_line(uint16_t x, uint16_t y, uint16_t y2, const Pixel &color)
{
    if(y > y2)
    {
        uint16_t temp = y;
        y = y2;
        y2 = temp;
    }
    for (; y < y2; y++) {
        CXfb::draw_pixel(x, y, color);
    }
}

void Graphics::put_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, const Pixel& color)
{
    if(x == x2)
    {
        put_vertical_line(x, y, y2, color);
        return;
    }
    else if(y == y2)
    {
        put_horizontal_line(x, x2, y, color);
        return;
    }
  int dx = abs(x-x2), sx = x<x2 ? 1 : -1;
  int dy = abs(y-y2), sy = y<y2 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    put_pixel(x,y, color);
    if (x==x2 && y==y2) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x += sx; }
    if (e2 < dy) { err += dx; y += sy; }
  }
}

void Graphics::put_rectangle(const Rect &rect, const Pixel &color)
{ 
    uint16_t xmax = min(320, rect.x + rect.width);
    uint16_t ymax = min(240, rect.y + rect.height);
    for(uint16_t x = rect.x; x < xmax; x++)
    {
        for(uint16_t y = rect.y; y < ymax; y++)
        {
            CXfb::draw_pixel(x, y, color.value);
        }
    }
}

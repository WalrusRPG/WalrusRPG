#include "Graphics.h"
#include "CXfb.h"
#include "stdio.h"

#define GRAPHICS WalrusRPG::Graphics

using namespace Nspire;

void GRAPHICS::init()
{
    CXfb::buffer_allocate();
}

void GRAPHICS::deinit()
{
    CXfb::buffer_free();
}

void GRAPHICS::frame_begin()
{
}

void GRAPHICS::frame_end()
{
    CXfb::buffer_swap_render();
}

void GRAPHICS::put_sprite(const Texture &sheet, int x, int y,
                          const WalrusRPG::Utils::Rect &window)
{
    CXfb::draw_sprite_sheet(sheet.data, x, y, window);
}

void GRAPHICS::put_sprite_tint(const Texture &sheet, int x, int y,
                     const WalrusRPG::Utils::Rect &window,
                     const WalrusRPG::Graphics::Pixel &color)
{
	CXfb::draw_sprite_sheet_tint(sheet.data, x, y, window, color.value);
}

void GRAPHICS::fill(const WalrusRPG::Graphics::Pixel &color)
{
    CXfb::buffer_fill(color);
}

void GRAPHICS::put_pixel(uint16_t x, uint16_t y, const WalrusRPG::Graphics::Pixel &color)

{
    CXfb::draw_pixel(x, y, color.value);
}

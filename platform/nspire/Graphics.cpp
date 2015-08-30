#include "Graphics.h"
#include "CXfb.h"
#include "stdio.h"

#define GRAPHICS WalrusRPG::Graphics

using namespace Nspire;

GRAPHICS::Pixel bg(0);

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
    CXfb::buffer_fill(bg);
}

void GRAPHICS::frame_end()
{
    CXfb::buffer_swap_render();
}

void GRAPHICS::put_sprite(const uint16_t *sheet, int x, int y,
                          const WalrusRPG::Utils::Rect &window)
{
    CXfb::draw_sprite_sheet(sheet, x, y, window);
}

void GRAPHICS::set_bg(const WalrusRPG::Graphics::Pixel &new_bg)
{
    bg = new_bg;
}

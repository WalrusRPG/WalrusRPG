#include "Graphics.h"
#include "render/Pixel.h"
#include "utility/Rect.h"
#include "Logger.h"
#include "utility/misc.h"
#include "utility/minmax.h"
#include <3ds.h>
#include <sf2d.h>

using namespace WalrusRPG; /*::Graphics*/
using WalrusRPG::Graphics::Pixel;
using WalrusRPG::Utils::Rect;

sf2d_rendertarget* target = nullptr;

inline u32 pixel2u32(const Pixel &pix)
{
  return RGBA8(pix.r<<3, pix.g<<2, pix.b<<3, 0xFF);
}

namespace {
  constexpr int OFFSET_X = 40;
  constexpr int OFFSET_Y = 0;
  void set_scissor()
  {
    sf2d_set_scissor_test(GPU_SCISSOR_NORMAL, 80, 0, 320, 240);
  }
}

void Graphics::init()
{
    // Logger::log("Graphics init");
    sf2d_init();
  	sf2d_set_clear_color(0);
  	sf2d_set_3D(0);
    sf2d_set_vblank_wait(1);
    set_scissor();
}

void Graphics::deinit()
{
    Logger::log("Graphics deinit");
    sf2d_free_target(target);
    sf2d_fini();
}

void Graphics::frame_begin()
{
  sf2d_start_frame(GFX_TOP, GFX_LEFT);
}

void Graphics::frame_end()
{
  sf2d_draw_rectangle(0, 0, OFFSET_X, 240, 0xFF000000);
  sf2d_draw_rectangle(320+OFFSET_X, 0, OFFSET_X, 240, 0xFF000000);
  sf2d_end_frame();
  sf2d_swapbuffers();
}

void Graphics::put_sprite(const Texture &sheet, int x, int y, const Rect &window)
{
    sf2d_draw_texture_part(sheet.data, x+OFFSET_X, y+OFFSET_Y, window.x, window.y, window.width, window.height);
}

void Graphics::put_sprite_tint(const Texture &sheet, int x, int y, const Rect &window,
                               const Pixel &color)
{
  sf2d_draw_texture_part_blend(sheet.data, x+OFFSET_X, y+OFFSET_Y, window.x, window.y, window.width, window.height, pixel2u32(color));
}

void Graphics::put_sprite_clipping(const Texture &sheet, int x, int y,
                                   const Rect &sprite_window, const Rect &clipping_window)
{
  sf2d_set_scissor_test(GPU_SCISSOR_NORMAL, clipping_window.x+OFFSET_X, clipping_window.y+OFFSET_Y, clipping_window.width, clipping_window.height);
  sf2d_draw_texture_part(sheet.data, x, y, sprite_window.x, sprite_window.y, sprite_window.width, sprite_window.height);
  set_scissor();
}


void Graphics::fill(const Pixel &color)
{
    sf2d_clear_target(target, pixel2u32(color));
}

void Graphics::put_pixel(uint16_t x, uint16_t y, const Pixel &color)
{
  sf2d_set_pixel(&(target->texture), x+OFFSET_X, y+OFFSET_Y, pixel2u32(color));
}

void Graphics::put_horizontal_line(uint16_t x, uint16_t x2, uint16_t y,
                                   const Pixel &color)
{
  // Because sf2dlib has issues with lines, let's port Nspire's functions.
  if (x > x2)
  {
      uint16_t temp = x;
      x = x2;
      x2 = temp;
  }
  for (; x <= x2; x++)
  {
      put_pixel(x+OFFSET_X, y+OFFSET_Y, color);
  }
}

void Graphics::put_vertical_line(uint16_t x, uint16_t y, uint16_t y2, const Pixel &color)
{
  // Because sf2dlib has issues with lines, let's port Nspire's functions.
  if (y > y2)
  {
      uint16_t temp = y;
      y = y2;
      y2 = temp;
  }
  for (; y <= y2; y++)
  {
      put_pixel(x+OFFSET_X, y+OFFSET_Y, color);
  }}

void Graphics::put_line(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2,
                        const Pixel &color)
{
  // Because sf2dlib has issues with lines, let's port Nspire's functions.
  if (x == x2)
  {
      put_vertical_line(x+OFFSET_X, y+OFFSET_Y, y2+OFFSET_Y, color);
      return;
  }
  else if (y == y2)
  {
      put_horizontal_line(x+OFFSET_X, x2+OFFSET_X, y+OFFSET_Y, color);
      return;
  }
  int dx = abs(x - x2), sx = x < x2 ? 1 : -1;
  int dy = abs(y - y2), sy = y < y2 ? 1 : -1;
  int err = (dx > dy ? dx : -dy) / 2, e2;

  for (;;)
  {
      put_pixel(x+OFFSET_X, y+OFFSET_Y, color);
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
  }}

void Graphics::put_rectangle(const Rect &rect, const Pixel &color)
{
  sf2d_draw_rectangle(rect.x+OFFSET_X, rect.y+OFFSET_Y, rect.width, rect.height, pixel2u32(color));
}

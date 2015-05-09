#include "sprites.h"
#include "Graphics.h"
#include "Text.h"
#include <string>

#define TEXT WalrusRPG::Graphics::Text
#define GRAPHICS WalrusRPG::Graphics

void TEXT::print_char(char c, unsigned x, unsigned y) {
  GRAPHICS::Rect_t rect;
  rect.w = 8;
  rect.h = 8;
  rect.x = (c%16)*8;
  rect.y = (c/16)*8;
  draw_sprite_sheet(font, x, y, &rect);
}

void TEXT::print_string(const char *str, unsigned x, unsigned y) {
  GRAPHICS::Rect_t rect;
  rect.w = 8;
  rect.h = 8;
  for(unsigned index = 0; str[index]; index++) {
    char c = str[index];
    rect.x = (c%16)*8;
    rect.y = (c/16)*8;
    draw_sprite_sheet(font, x, y, &rect);
    x +=8;
  }
}

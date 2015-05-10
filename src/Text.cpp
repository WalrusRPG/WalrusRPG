#include "sprites.h"
#include "Graphics.h"
#include "Text.h"
#include <cstdio>
#include <cstdarg>
#include <string>

#define TEXT WalrusRPG::Graphics::Text
#define GRAPHICS WalrusRPG::Graphics

void TEXT::print_char(char c, unsigned x, unsigned y)
{
    GRAPHICS::Rect_t rect;
    rect.w = 8;
    rect.h = 8;
    rect.x = (c % 16) * 8;
    rect.y = (c / 16) * 8;
    draw_sprite_sheet(font, x, y, &rect);
}

void TEXT::print_string(const char *str, unsigned x, unsigned y)
{
    GRAPHICS::Rect_t rect;
    rect.w = 8;
    rect.h = 8;
    for (unsigned index = 0; str[index]; index++)
    {
        char c = str[index];
        rect.x = (c % 16) * 8;
        rect.y = (c / 16) * 8;
        draw_sprite_sheet(font, x, y, &rect);
        x += 8;
    }
}

void TEXT::print_string(const std::string &str, unsigned x, unsigned y)
{
    TEXT::print_string(str.c_str(), x, y);
}

void TEXT::print_format(unsigned x, unsigned y, const char *format, ...)
{
    char buffer[256] =
        "";
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 256, format, args);
    print_string(buffer, x, y);
}

void TEXT::print_format(unsigned x, unsigned y, const std::string &format, ...)
{
    TEXT::print_format(x, y, format.c_str());
}

#include <cstdio>
#include <cstdarg>
#include "Text.h"
#include "sprites.h"
#include "Graphics.h"

using namespace WalrusRPG::Graphics; /*Text*/
using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::Utils;

Texture tex_font((char *) font);

void Text::print_char(char c, unsigned x, unsigned y)
{
    put_sprite(tex_font, x, y, Rect((c % 16) * 8, (c / 16) * 8, 8, 8));
}

void Text::print_string(const char *str, unsigned x, unsigned y)
{
    Rect rect;
    rect.width = 8;
    rect.height = 8;
    for (unsigned index = 0; str[index]; index++)
    {
        char c = str[index];
        rect.x = (c % 16) * 8;
        rect.y = (c / 16) * 8;
        put_sprite(tex_font, x, y, rect);
        x += 8;
    }
}

void Text::print_string(const std::string &str, unsigned x, unsigned y)
{
    Text::print_string(str.c_str(), x, y);
}

void Text::print_format(unsigned x, unsigned y, const char *format, ...)
{
    char buffer[256] = "";
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 256, format, args);
    print_string(buffer, x, y);
}

void Text::print_format(unsigned x, unsigned y, const std::string &format, ...)
{
    Text::print_format(x, y, format.c_str());
}

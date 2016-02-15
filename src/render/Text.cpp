#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "Text.h"
#include "Graphics.h"
#include "render/Font.h"
#include "piaf/Archive.h"

using namespace WalrusRPG::Graphics; /*Text*/
using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::Utils;
using namespace WalrusRPG::PIAF;

Font* fnt;
Texture* tex;

void Text::init()
{
    Archive arc("data/out.wrf");
    tex = new Texture(arc.get("t_dbgfnt"));
    fnt = new Font(*tex, arc.get("f_dbgfnt"));
}

void Text::deinit()
{
    delete fnt;
    delete tex;
}

void Text::print_char(char c, unsigned x, unsigned y)
{
    put_rectangle({static_cast<int>(x), static_cast<int>(y), 8, 8}, Black);
    fnt->draw(x, y, c);
    //put_sprite(tex_font, x, y, Rect((c % 16) * 8, (c / 16) * 8, 8, 8));
}

void Text::print_string(const char *str, unsigned x, unsigned y)
{
    put_rectangle({static_cast<int>(x), static_cast<int>(y), static_cast<unsigned>(8*strlen(str)), 8}, Black);
    fnt->draw(x, y, str);
    // Rect rect;
    // rect.width = 8;
    // rect.height = 8;
    // for (unsigned index = 0; str[index]; index++)
    // {
    //     char c = str[index];
    //     rect.x = (c % 16) * 8;
    //     rect.y = (c / 16) * 8;
    //     put_sprite(tex_font, x, y, rect);
    //     x += 8;
    // }
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

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "Text.h"
#include "Graphics.h"
#include "engine/ResourceManager.h"
#include "render/Font.h"
#include "piaf/Archive.h"

using namespace WalrusRPG::Graphics; /*Text*/
using namespace WalrusRPG::ResourceManager;
using namespace WalrusRPG::Utils;
using namespace WalrusRPG::PIAF;

Font *fnt;
Texture *tex;

void Text::init()
{
    printf("Initiating Text\n");
    WalrusRPG::ManagedArchive m ("data/out.wrf");
    Archive *arc(m);
    tex = new Texture(arc->get("t_dbgfnt"));
    fnt = new Font(*tex, arc->get("f_dbgfnt"));
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
}

void Text::print_string(const char *str, unsigned x, unsigned y)
{
    put_rectangle({static_cast<int>(x), static_cast<int>(y),
                   static_cast<unsigned>(8 * strlen(str)), 8},
                  Black);
    fnt->draw(x, y, str);
}

inline void Text::print_string(const std::string &str, unsigned x, unsigned y)
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

inline void Text::print_format(unsigned x, unsigned y, const std::string &format, ...)
{
    Text::print_format(x, y, format.c_str());
}

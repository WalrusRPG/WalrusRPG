#include "StateMap.h"
#include "Graphics.h"
#include "render/Text.h"
#include "piaf/Archive.h"


using WalrusRPG::States::StateMap;
using namespace WalrusRPG;
using namespace WalrusRPG::Graphics;
using WalrusRPG::Utils::Rect;
using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File;
using WalrusRPG::Graphics::Texture;

namespace
{

    void print_debug_camera_data(const Camera &camera)
    {
        Text::print_format(0, 8, "CAM : X : %d Y: %d", camera.get_x(), camera.get_y());
    }

    void print_debug_map_data(const Map &map)
    {
        Text::print_format(0, 16, "MAP : W: %d, H:%d", map.get_width(), map.get_height());
    }
}

void StateMap::putchar_haeccity(unsigned char c, unsigned x, unsigned y)
{
    // Rect letter{haeccity[c][0], haeccity[c][1], haeccity[c][2], haeccity[c][3]};
    // put_sprite(tex_haeccity, x + haeccity[c][4], y + haeccity[c][5], letter);
    put_sprite(txt.font_tex, x + txt.chars[c].x_offset, y + txt.chars[c].y_offset, txt.chars[c].dimensions);
}

void StateMap::putchar_haeccity_tint(unsigned char c, unsigned x, unsigned y,
                                     const Pixel &col)
{
    put_sprite_tint(txt.font_tex, x + txt.chars[c].x_offset, y + txt.chars[c].y_offset, txt.chars[c].dimensions, col);
}


void StateMap::print_haeccity(const char *str, unsigned x, unsigned y,
                              const Pixel &col = White)
{
    const bool must_tint = (col != White);
    for (unsigned i = 0; str[i] && x < 320; i++)
    {
        unsigned char c = str[i];
        // printf("%c = %d\n", c, c);
        if (must_tint)
            putchar_haeccity_tint(c, x, y, col);
        else
            putchar_haeccity(c, x, y);
        x += txt.chars[c].dimensions.width + 1;
    }
}

// TODO : We definitely need a Resource Manager
StateMap::StateMap(int x, int y, Map &map)
    : camera(x, y), map(map), data("data/out.wrf"),
      tex_haeccity(data.get("t_haecci")), txt(tex_haeccity, data.get("f_haecci"))
{
}

void StateMap::update(unsigned dt)
{
    camera.update(dt);
}

void StateMap::render(unsigned dt)
{
    // fill(Black);
    map.render(camera, dt);

    // print_debug_camera_data(camera);
    // print_debug_map_data(map);
    print_haeccity("Hello world! :D", 0, 0);
    print_haeccity("This isn't actually a utility nor built-ins functions.", 0, 9);
    print_haeccity("This is a quick prototype to see if variable-wdith fonts works.", 0,
                   18);
    print_haeccity("Builting it myself allows me to make it work on calc too.", 0, 27,
                   Yellow);
    print_haeccity(
        "( I wonder if a conditionnal to tint the text slows down a lot the process. )",
        0, 36, Gray);
    print_haeccity("Oh well, I hope I'll get to do a pretty textbox like in games !", 0,
                   45);

    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            putchar_haeccity_tint(16 * j + i, 6 * i, 9 * j + 54,
                                  Pixel(i * 16, j * 16, 255));
        }
    }
}

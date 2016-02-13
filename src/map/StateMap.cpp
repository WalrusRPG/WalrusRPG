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

// TODO : We definitely need a Resource Manager
StateMap::StateMap(int x, int y, Map &map)
    : camera(x, y), map(map), data("data/out.wrf"),
      tex_haeccity(data.get("t_haeccity")), txt(tex_haeccity, data.get("f_haeccity"))
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
    txt.draw("Hello world! :D", 0, 0);
    txt.draw("This isn't actually a utility nor built-ins functions.", 0, 9);
    txt.draw("This is a quick prototype to see if variable-wdith fonts works.", 0,
                   18);
    txt.draw("Builting it myself allows me to make it work on calc too.", 0, 27,
                   Yellow);
    txt.draw(
        "( I wonder if a conditionnal to tint the text slows down a lot the process. )",
        0, 36, Gray);
    txt.draw("Oh well, I hope I'll get to do a pretty textbox like in games !", 0,
                   45);

    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 16; ++j)
        {
            txt.draw(16 * j + i, 6 * i, 9 * j + 54,
                                  Pixel(i * 16, j * 16, 255));
        }
    }
}

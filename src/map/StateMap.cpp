#include "StateMap.h"
#include "Graphics.h"
#include "render/Text.h"
#include "piaf/Archive.h"
#include "engine/ResourceManager.h"

using WalrusRPG::States::StateMap;
using namespace WalrusRPG;
using namespace WalrusRPG::Graphics;
using WalrusRPG::Utils::Rect;
using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Graphics::Font;

namespace
{
    void print_debug_camera_data(const Camera &camera, const Font &fnt)
    {
        fnt.draw_format(240, 1, Black, "CAM : X : %d Y: %d", camera.get_x(),
                        camera.get_y());
        fnt.draw_format(240, 0, "CAM : X : %d Y: %d", camera.get_x(), camera.get_y());
    }

    void print_debug_map_data(const Map &map, const Font &fnt)
    {
        fnt.draw_format(240, 9, Black, "MAP : W: %d, H:%d", map.get_width(),
                        map.get_height());
        fnt.draw_format(240, 8, "MAP : W: %d, H:%d", map.get_width(), map.get_height());
    }
}

// TODO : We definitely need a Resource Manager
StateMap::StateMap(int x, int y, Map &map)
    : camera(x, y), map(map), data(ManagedArchive("data/out.wrf")), tex_haeccity(data->get("t_haeccity")),
      txt(tex_haeccity, data->get("f_haeccity"))
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

    print_debug_camera_data(camera, txt);
    print_debug_map_data(map, txt);
}

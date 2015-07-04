#include "StateMap.h"
#include "misc.h"
#include "version.h"
#include "Graphics.h"
#include "Pixel.h"
#include "Text.h"

#define STATEMAP WalrusRPG::States::StateMap

using namespace WalrusRPG;

namespace
{
    void print_debug_camera_data(const Camera &camera)
    {
        Graphics::Text::print_format(0, 8, "CAM : X : %d Y: %d", camera.get_x(),
                                     camera.get_y());
    }

    void print_debug_map_data(const Map &map)
    {
        Graphics::Text::print_format(0, 16, "MAP : W: %d, H:%d", map.get_width(),
                                     map.get_height());
    }
}

STATEMAP::StateMap(int x, int y, Map &map) : camera(x, y), map(map)
{
}

void STATEMAP::update(unsigned dt)
{
    camera.update(dt);
}

void STATEMAP::render(unsigned dt)
{
    Graphics::Pixel pix(Graphics::Green);
    Graphics::buffer_fill(pix);
    map.render(camera, dt);

    print_debug_camera_data(camera);
    print_debug_map_data(map);
}

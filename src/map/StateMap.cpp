#include "StateMap.h"
#include "Graphics.h"
#include "render/Text.h"

#include "imgui.h"
#include "imgui-events-SFML.h"
#include "imgui-rendering-SFML.h"

using WalrusRPG::States::StateMap;
using namespace WalrusRPG;
using namespace WalrusRPG::Graphics;

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

StateMap::StateMap(int x, int y, Map &map) : camera(x, y), map(map)
{
    camera.set_x(0);
}

void StateMap::update(unsigned dt)
{
    camera.update(dt);
}

void StateMap::render(unsigned dt)
{
    bool tmp = true;
    ImGuiIO &io = ImGui::GetIO();
    ImGui::Begin("Imgui PoC window", &tmp);
    ImGui::Text("Hello, world!");
    ImGui::End();
    map.render(camera, dt);

    print_debug_camera_data(camera);
    print_debug_map_data(map);
}

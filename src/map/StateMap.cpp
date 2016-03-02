#include "StateMap.h"
#include "Graphics.h"
#include "render/Text.h"

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
#if TARGET_SFML
    active_map_mode = 0;
#endif
}

void StateMap::update(unsigned dt)
{
    camera.update(dt);
}

void StateMap::render(unsigned dt)
{
    map.render(camera, dt);

    // print_debug_camera_data(camera);
    print_debug_map_data(map);
}

#if TARGET_SFML
void StateMap::debug(unsigned dt)
{
    ImGui::Begin("Map State");
    ImGui::BeginGroup();
    ImGui::Text("Camera");
    ImGui::Indent();
    ImGui::Value("X", camera.get_x());
    ImGui::SameLine();
    ImGui::Value("Y", camera.get_y());
    ImGui::EndGroup();

    ImGui::Separator();

    ImGui::BeginGroup();
    ImGui::Text("Map");
    ImGui::Indent();
    ImGui::Value("W", map.get_width());
    ImGui::Unindent();
    ImGui::SameLine();
    ImGui::Value("H", map.get_height());
    int value;
    if(ImGui::RadioButton("Solid", active_map_mode == 0)) active_map_mode = 0;
    ImGui::SameLine();
    if(ImGui::RadioButton("Animated", active_map_mode == 1)) active_map_mode = 1;

    ImVec2 s = ImGui::GetCursorScreenPos();
    ImDrawList *list = ImGui::GetWindowDrawList();
    for(signed i = 0; i < map.get_height(); i++)
    {
        for(signed j = 0; j < map.get_width(); j++)
        {
            float x = s.x + 3*j;
            float y = s.y + 3*i;
            uint16_t tile = map.layer0[i*map.get_height() + j];
            if(active_map_mode == 0)
            {
                if(tile != 0)
                {
                    ImU32 c{0x88888888};
                    list->AddRectFilled({x, y},{x+3, y+3}, c);
                }
                if(map.layer1 != nullptr)
                {
                    uint16_t tile2 = map.layer1[i*map.get_height() + j];
                    if(tile2 != 0)
                    {
                        ImU32 c{0xFF0000FF};
                        list->AddRectFilled({x, y},{x+3, y+3}, c);
                    }
                }
            }
            else if(active_map_mode == 1)
            {
                auto ptr = map.anim.animations.find(tile);
                if(ptr->second.stripe.size() > 1)
                    list->AddRectFilled({x, y},{x+3, y+3}, 0xFFFF0000+ map.anim.get_animation_frame(tile));                
            }
        }
        float x = camera.get_x()/(float)16;
        float x2 = (camera.get_x() + 320)/(float)16;
        float y = camera.get_y()/(float)16;
        float y2 = (camera.get_y()+240)/(float)16;
        list->AddRect({x*3+s.x, y*3+s.y}, {x2*3+s.x, y2*3+s.y}, 0xFFFFFF00);

    }
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + map.get_height()*3 + ImGui::GetStyle().ItemSpacing.y);
    ImGui::EndGroup();
    ImGui::End();
}
#endif

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
    : camera(x, y), map(map), data("data/out.wrf"), tex_haeccity(data.get("t_haeccity")),
      txt(tex_haeccity, data.get("f_haeccity"))
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
    // fill(Black);
    map.render(camera, dt);

    // print_debug_camera_data(camera);
    print_debug_map_data(map, txt);
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
    if (ImGui::RadioButton("Solid", active_map_mode == 0))
        active_map_mode = 0;
    ImGui::SameLine();
    if (ImGui::RadioButton("Animated", active_map_mode == 1))
        active_map_mode = 1;

    ImVec2 s = ImGui::GetCursorScreenPos();
    ImDrawList *list = ImGui::GetWindowDrawList();

    if (map.layer0 != nullptr)
    {
        for (signed i = 0; i < map.get_height(); i++)
        {
            for (signed j = 0; j < map.get_width(); j++)
            {
                float x = s.x + 3 * j;
                float y = s.y + 3 * i;
                uint16_t tile = map.layer0[i * map.get_height() + j];
                if (active_map_mode == 0)
                {
                    if (tile != 0)
                    {
                        ImU32 c{0x88888888};
                        list->AddRectFilled({x, y}, {x + 3, y + 3}, c);
                    }
                }
                else if (active_map_mode == 1)
                {
                    auto ptr = map.anim.animations.find(tile);
                    if (ptr->second.stripe.size() > 1)
                        list->AddRectFilled({x, y}, {x + 3, y + 3},
                                            0xFFFF0000 +
                                                map.anim.get_animation_frame(tile));
                }
            }
        }
    }
    if (map.layer1 != nullptr)
    {
        for (signed i = 0; i < map.get_height(); i++)
        {
            for (signed j = 0; j < map.get_width(); j++)
            {
                float x = s.x + 3 * j;
                float y = s.y + 3 * i;
                uint16_t tile2 = map.layer1[i * map.get_height() + j];
                if (active_map_mode == 0)
                {
                    if (tile2 != 0)
                    {
                        ImU32 c{0xFF0000FF};
                        list->AddRectFilled({x, y}, {x + 3, y + 3}, c);
                    }
                }
                else if (active_map_mode == 1)
                {
                    if (tile2 != 0)
                    {
                        ImU32 c{0xFF00FF00};
                        auto ptr = map.anim.animations.find(tile2);
                        if (ptr->second.stripe.size() > 1)
                            list->AddRectFilled({x, y}, {x + 3, y + 3}, c);
                    }
                }
            }
        }
    }

    float x = camera.get_x() / (float) 16;
    float x2 = (camera.get_x() + 320) / (float) 16;
    float y = camera.get_y() / (float) 16;
    float y2 = (camera.get_y() + 240) / (float) 16;
    list->AddRect({floorf(x * 3 + s.x), floorf(y * 3 + s.y)},
                  {floorf(x2 * 3 + s.x), floorf(y2 * 3 + s.y)}, 0xFFFFFF00);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + map.get_height() * 3 +
                         ImGui::GetStyle().ItemSpacing.y);
    ImGui::EndGroup();
    ImGui::End();
}
#endif

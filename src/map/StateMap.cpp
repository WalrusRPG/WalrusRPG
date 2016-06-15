#include <cmath>
#include "StateMap.h"
#include "Graphics.h"
#include "input/Input.h"
#include "render/Text.h"
#include "piaf/Archive.h"
#include "Logger.h"

using WalrusRPG::States::StateMap;
using namespace WalrusRPG;
using namespace WalrusRPG::Graphics;
using WalrusRPG::Utils::Rect;
using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File;
using WalrusRPG::Graphics::Texture;
using namespace WalrusRPG::Input;
using WalrusRPG::Input::Key;
using WalrusRPG::Graphics::Font;
using WalrusRPG::Textbox;

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
    : started(false), camera(x, y), map(map), data("data/wip_data.wrf"),
      tex_haeccity((*data).get("t_haeccity")),
      txt(tex_haeccity, (*data).get("f_haeccity")), box(txt)
{
#if TARGET_SFML
    active_map_mode = 0;
#endif
    box.set_text((
        char *) "Hello world! I am "
                "\xFF\x01\xf0\x00\x00Howard\xFF\x01\xff\xff\x00"
                ".\n"
                "\xFF\x81\x78\x00\x00"
                "\xFF\x81\x0a\x00\x00"
                "\xFF\x01\xf0\x00\x00"
                "Howard"
                "\xFF\x01\xff\xff\x00"
                " the Psyduck!\n"
                "How goes? I'm fine, headache-y but fine. Heh, it's normal, don't worry "
                "for me.\n"
                "I wonder... Heh, Let's see if it works correctly, shall we?");
}

StateMap::~StateMap()
{
}

void StateMap::update(unsigned dt)
{
    unsigned t = dt * (key_down(K_B) ? 3 : 1);
    if (key_pressed(K_A))
    {
        if (!started && box.state != Done)
            started = true;
        else if (box.state == Done)
        {
            started = false;
        }
    }
    if (started)
        box.update(t);
    camera.update(t);
}

void StateMap::render(unsigned dt)
{
    map.render(camera, dt);

// print_debug_camera_data(camera);
#if !TARGET_SFML
    print_debug_map_data(map, txt);
#endif
    print_debug_camera_data(camera, txt);
    if (!started)
        return;
    box.render(dt);
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

    if (ImGui::RadioButton("Layers", active_map_mode == 0))
        active_map_mode = 0;
    if (ImGui::RadioButton("Animated", active_map_mode == 1))
        active_map_mode = 1;

    if (ImGui::BeginChild("map_frame", {0, 0}, true))
    {
        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() &&
            ImGui::IsMouseDragging(1, 0.0f))
            scrolling = {scrolling.x - ImGui::GetIO().MouseDelta.x,
                         scrolling.y - ImGui::GetIO().MouseDelta.y};

        ImVec2 s = ImGui::GetCursorScreenPos();
        s = {s.x - scrolling.x, s.y - scrolling.y};
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
        list->AddRect({std::floor(x * 3 + s.x), std::floor(y * 3 + s.y)},
                      {std::floor(x2 * 3 + s.x), std::floor(y2 * 3 + s.y)}, 0xFFFFFF00);

        ImGui::EndChild();
    }
    ImGui::EndGroup();
    ImGui::End();
}
#endif
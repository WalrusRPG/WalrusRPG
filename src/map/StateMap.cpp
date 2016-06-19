#include <cmath>
#include "StateMap.h"
#include "Graphics.h"
#include "input/Input.h"
#include "render/Text.h"
#include "collision/Collision.h"
#include "piaf/Archive.h"
#include "Logger.h"
#include "render/TileRenderer.h"
#include "TalkEntity.h"

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
using WalrusRPG::Entity;
using WalrusRPG::TileRenderer;
using WalrusRPG::TalkEntity;

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
      txt(tex_haeccity, (*data).get("f_haeccity")), box(txt),
      p(*this, 32, 40, 10, 4,
        new TileRenderer(map.tmap.get_texture(), Tileset::TILE_DIMENSION,
                         Tileset::TILE_DIMENSION),
        128)
{
    map.add_entity(&p);
    TileRenderer *tr = new TileRenderer(map.tmap.get_texture(), Tileset::TILE_DIMENSION,
                                        Tileset::TILE_DIMENSION);
    map.add_entity(new TalkEntity(*this, 128, 64, Tileset::TILE_DIMENSION,
                                  Tileset::TILE_DIMENSION, tr, 150,
                                  "Hello, I'm a skeleton."));
    map.add_entity(new TalkEntity(*this, 128, 96, Tileset::TILE_DIMENSION,
                                  Tileset::TILE_DIMENSION, tr, 134,
                                  "Hello, I'm another skeleton."));
    map.add_entity(new TalkEntity(
        *this, 138, 104, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr, 134,
        "Doot doot. Thanks \xFF\x02\xFF\x00\x00Mr. Skeltal\xFF\x02\xFF\xFF\xFF!"));
    /*
    map.add_entity(
        new Entity(*this, 196, 112, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr,
    134));
    map.add_entity(
        new Entity(*this, 196, 104, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr,
    134));
    */
#if TARGET_SFML
    active_map_mode = 0;
#endif
}

StateMap::~StateMap()
{
}

void StateMap::update()
{
    camera.set_center_x(p.x + p.w / 2);
    camera.set_center_y(p.y + p.h / 2);

    unsigned t = (key_down(K_B) ? 16 : 1);
    map.update();

    if (started)
    {
        for (unsigned i = 0; i < t; i++)
            box.update();
        if (key_pressed(K_A) && box.state == Done)
        {
            started = false;
            p.controllable = true;
        }
    }
    else
    {
        if (key_pressed(K_A))
        {
            // Check direction.
            Rect check_hitbox;
            switch (p.direction)
            {
                // up
                case 0:
                    check_hitbox = {(int) p.x, (int) p.y - 4, p.w, 4};
                    break;
                // down
                case 1:
                    check_hitbox = {(int) p.x, (int) p.y + (int) p.h, p.w, 4};
                    break;
                // left
                case 2:
                    check_hitbox = {(int) p.x - (int) p.w - 4, (int) p.y, 4, p.h};
                    break;
                // down
                case 3:
                    check_hitbox = {(int) p.x + (int) p.w, (int) p.y, 4, p.h};
                    break;
            }
            // Check
            for (auto ptr = map.entities.begin(); ptr < map.entities.end(); ptr++)
            {
                Entity *e = *ptr;
                if (e == &p)
                    continue;
                if (WalrusRPG::AABBCheck(check_hitbox,
                                         {(int) e->x, (int) e->y, e->w, e->h}))
                {
                    e->interact_with(p, InteractionType::CHECK);
                    Logger::log("Interacted with %p", e);
                }
            }
            // if (!started && box.state != Done)
            //     started = true;
            // else if (box.state == Done)
            // {
            // }
        }
    }
    camera.update();
}

void StateMap::render()
{
    map.render(camera);
    print_debug_camera_data(camera, txt);
#if !TARGET_SFML
    print_debug_map_data(map, txt);
#endif
    if (started)
        box.render();
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
                    uint16_t tile = map.layer0[i * map.get_width() + j];
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
                        if (ptr != map.anim.animations.end() && ptr->second.stripe.size() > 1)
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
                    uint16_t tile2 = map.layer1[i * map.get_width() + j];
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
                            if (ptr != map.anim.animations.end() && ptr->second.stripe.size() > 1)
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
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
#include "DoorEntity.h"

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
using WalrusRPG::DoorEntity;
using WalrusRPG::MAP_OBJECT_GRID_TILE_WIDTH;

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
        new TileRenderer(map.tmap.get_texture(), TILE_DIMENSION, TILE_DIMENSION), 128)
{
    map.add_entity(&p);
    TileRenderer *tr =
        new TileRenderer(map.tmap.get_texture(), TILE_DIMENSION, TILE_DIMENSION);
    map.add_entity(new TalkEntity(*this, 128, 64, TILE_DIMENSION, TILE_DIMENSION, tr, 150,
                                  "Hello, I'm a skeleton."));
    map.add_entity(new TalkEntity(*this, 128, 96, TILE_DIMENSION, TILE_DIMENSION, tr, 134,
                                  "Hello, I'm another skeleton."));
    map.add_entity(new TalkEntity(
        *this, 138, 104, TILE_DIMENSION, TILE_DIMENSION, tr, 134,
        "Doot doot. Thanks \xFF\x02\xFF\x00\x00Mr. Skeltal\xFF\x02\xFF\xFF\xFF!"));


    map.add_entity(new TalkEntity(
        *this, 138, 104, TILE_DIMENSION, TILE_DIMENSION, tr, 134,
        "Doot doot. Thanks \xFF\x02\xFF\x00\x00Mr. Skeltal\xFF\x02\xFF\xFF\xFF!"));


    map.add_entity(
        new DoorEntity(*this, 48, 240, TILE_DIMENSION, TILE_DIMENSION, tr, 4, 7));
    map.add_entity(
        new DoorEntity(*this, 352, 240, TILE_DIMENSION, TILE_DIMENSION, tr, 4, 7));

    map.reset_entity_grid();
    /*
    map.add_entity(
        new Entity(*this, 196, 112, TILE_DIMENSION, TILE_DIMENSION, tr,
    134));
    map.add_entity(
        new Entity(*this, 196, 104, TILE_DIMENSION, TILE_DIMENSION, tr,
    134));
    */
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
                    map.remove_entity_from_grid(e);
                    map.add_entity_to_grid(e);
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
#if !IMGUI
    print_debug_map_data(map, txt);
#endif
    if (started)
        box.render();
}

#if IMGUI

void StateMap::debug_layer(uint16_t *layer, ImU32 color, ImDrawList *list, ImVec2 offset)
{
    for (signed i = 0; i < map.get_height(); i++)
    {
        for (signed j = 0; j < map.get_width(); j++)
        {
            float x = offset.x + 4 * j;
            float y = offset.y + 4 * i;
            uint16_t tile = layer[i * map.get_width() + j];
            {
                if (tile != 0)
                {
                    list->AddRectFilled({x, y}, {x + 4, y + 4}, color);
                }
            }
        }
    }
}

void StateMap::debug()
{
    if (!ImGui::Begin("Map state"))
        return;
    ImGui::BeginGroup();
    ImGui::Text("Camera");
    ImGui::Indent();
    ImGui::Value("X", camera.get_x());
    ImGui::SameLine();
    ImGui::Value("Y", camera.get_y());
    ImGui::EndGroup();
    ImGui::Separator();

    ImGui::Text("Map");
    ImGui::Indent();
    ImGui::Value("W", map.get_width());
    ImGui::SameLine();
    ImGui::Value("H", map.get_height());
    ImGui::Value("Nb entities", (int) map.entities.size());
    ImGui::Unindent();

    ImGui::Text("Layers");
    if (ImGui::Checkbox("Ground", &show_layer_ground))
        ;
    ImGui::SameLine();
    if (ImGui::Checkbox("Middle", &show_layer_middle))
        ;
    ImGui::SameLine();
    if (ImGui::Checkbox("Over", &show_layer_over))
        ;
    if (ImGui::Checkbox("Entities", &show_entities))
        ;
    if (show_entities)
    {
        ImGui::SameLine();
        ImGui::Checkbox("Grid", &show_entity_grid);
    }

    if (ImGui::BeginChild("map_frame", {0, 0}, true))
    {
        if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() &&
            ImGui::IsMouseDragging(1, 0.0f))
            scrolling = {scrolling.x - ImGui::GetIO().MouseDelta.x,
                         scrolling.y - ImGui::GetIO().MouseDelta.y};

        ImVec2 s = ImGui::GetCursorScreenPos();
        s = {s.x - scrolling.x, s.y - scrolling.y};
        ImDrawList *list = ImGui::GetWindowDrawList();
        if (show_layer_ground && map.layer0 != nullptr)
            StateMap::debug_layer(map.layer0, 0x88888888, list, s);
        if (show_layer_middle && map.layer1 != nullptr)
            StateMap::debug_layer(map.layer1, 0xFF0000FF, list, s);
        if (show_entities)
        {
            for (auto ptr = map.entities.begin(); ptr != map.entities.end(); ptr++)
            {
                Entity *e = *ptr;
                float x = s.x + e->x / TILE_DIMENSION * 4;
                float x2 = s.x + (e->x + e->w) / TILE_DIMENSION * 4;
                float y = s.y + e->y / TILE_DIMENSION * 4;
                float y2 = s.y + (e->y + e->h) / TILE_DIMENSION * 4;
                list->AddRectFilled({x, y}, {x2, y2}, 0xFFFFFFFF);
            }
            // Entity Grid
            if (show_entity_grid)
            {
                for (int i = 0, max_i = map.get_height() / MAP_OBJECT_GRID_TILE_WIDTH + 1;
                     i < max_i; ++i)
                {
                    for (int j = 0,
                             max_j = map.get_width() / MAP_OBJECT_GRID_TILE_WIDTH + 1;
                         j < max_j; ++j)
                    {
                        list->AddRect(
                            {std::floor(j * MAP_OBJECT_GRID_TILE_WIDTH * 4 + s.x),
                             std::floor(i * MAP_OBJECT_GRID_TILE_WIDTH * 4 + s.y)},
                            {std::floor((j + 1) * MAP_OBJECT_GRID_TILE_WIDTH * 4 + s.x),
                             std::floor((i + 1) * MAP_OBJECT_GRID_TILE_WIDTH * 4 + s.y)},
                            0x80FFFFFF);
                    }
                }
            }
        }
        if (show_layer_over && map.layer2 != nullptr)
            StateMap::debug_layer(map.layer2, 0x800000FF, list, s);
        float x = camera.get_x() / (float) TILE_DIMENSION;
        float x2 = (camera.get_x() + 320) / (float) TILE_DIMENSION;
        float y = camera.get_y() / (float) TILE_DIMENSION;
        float y2 = (camera.get_y() + 240) / (float) TILE_DIMENSION;
        list->AddRect({std::floor(x * 4 + s.x), std::floor(y * 4 + s.y)},
                      {std::floor(x2 * 4 + s.x), std::floor(y2 * 4 + s.y)}, 0xFFFFFF00);
    }
    ImGui::EndChild();
    if (show_entity_grid)
    {
        ImGui::Text("Grid entity population");
        for (int i = 0, max_i = map.get_height() / MAP_OBJECT_GRID_TILE_WIDTH + 1;
             i < max_i; ++i)
        {
            for (int j = 0, max_j = map.get_width() / MAP_OBJECT_GRID_TILE_WIDTH + 1;
                 j < max_j; ++j)
            {
                ImGui::Text("%3d", map.entity_container[i][j].size());
                ImGui::SameLine();
            }
            ImGui::Text("");
        }
    }

    ImGui::End();
}
#endif
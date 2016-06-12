#include "StateMap.h"
#include "Graphics.h"
#include "input/Input.h"
#include "render/Text.h"
#include "collision/Collision.h"
#include "piaf/Archive.h"
#include "engine/ResourceManager.h"
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
    : started(false), camera(x, y), map(map), data(ManagedArchive("data/wip_data.wrf")),
      tex_haeccity(data->get("t_haeccity")), txt(tex_haeccity, data->get("f_haeccity")),
      box(txt), p(*this, 32, 40, 10, 4,
                  new TileRenderer(map.tmap.get_texture(), Tileset::TILE_DIMENSION,
                                   Tileset::TILE_DIMENSION),
                  128)
{
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
    map.add_entity(&p);
    TileRenderer *tr = new TileRenderer(map.tmap.get_texture(), Tileset::TILE_DIMENSION,
                                        Tileset::TILE_DIMENSION);
    map.add_entity(new TalkEntity(*this, 128, 64, Tileset::TILE_DIMENSION,
                                  Tileset::TILE_DIMENSION, tr, 150,
                                  "Hello, I'm a skeleton."));
    map.add_entity(new TalkEntity(*this, 128, 96, Tileset::TILE_DIMENSION,
                                  Tileset::TILE_DIMENSION, tr, 134,
                                  "Hello, I'm another skeleton."));
    /*
    map.add_entity(
        new Entity(*this, 136, 104, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr,
    134));
    map.add_entity(
        new Entity(*this, 196, 112, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr,
    134));
    map.add_entity(
        new Entity(*this, 196, 104, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr,
    134));
    */
}


void StateMap::update(unsigned dt)
{
    camera.set_center_x(p.x + p.w / 2);
    camera.set_center_y(p.y + p.h / 2);

    unsigned t = dt * (key_down(K_B) ? 16 : 1);
    if (started)
    {
        box.update(t);
        if (key_pressed(K_A) && box.state == Done)
            started = false;
    }
    else
    {
        map.update(dt);
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
    camera.update(t);
}

void StateMap::render(unsigned dt)
{
    map.render(camera, dt);
    print_debug_camera_data(camera, txt);
    print_debug_map_data(map, txt);
    if (started)
        box.render(dt);
}

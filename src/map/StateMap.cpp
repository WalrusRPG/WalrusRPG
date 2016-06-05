#include "StateMap.h"
#include "Graphics.h"
#include "input/Input.h"
#include "render/Text.h"
#include "piaf/Archive.h"
#include "engine/ResourceManager.h"
#include "Logger.h"
#include "render/TileRenderer.h"

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
      box(txt),
      p(32, 40, 10, 4, new TileRenderer(map.tmap.get_texture(), Tileset::TILE_DIMENSION,
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
    map.add_entity(
        new Entity(128, 64, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr, 150));
    map.add_entity(
        new Entity(128, 96, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr, 134));
    map.add_entity(
        new Entity(136, 104, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr, 134));
    map.add_entity(
        new Entity(196, 112, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr, 134));
    map.add_entity(
        new Entity(196, 104, Tileset::TILE_DIMENSION, Tileset::TILE_DIMENSION, tr, 134));
}


void StateMap::update(unsigned dt)
{
    camera.set_center_x(p.x + p.w / 2);
    camera.set_center_y(p.y + p.h / 2);

    unsigned t = dt * (key_down(K_B) ? 16 : 1);
    map.update(dt);

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
    print_debug_camera_data(camera, txt);
    print_debug_map_data(map, txt);
    if (!started)
        return;
    box.render(dt);
}

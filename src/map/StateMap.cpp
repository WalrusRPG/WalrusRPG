#include "StateMap.h"
#include "Graphics.h"
#include "input/Input.h"
#include "render/Text.h"
#include "piaf/Archive.h"

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
    : started(false), camera(x, y), map(map), data("data/out.wrf"), tex_haeccity(data.get("t_haecci")),
      txt(tex_haeccity, data.get("f_haecci")), tex_psyduck(data.get("psyduck")), box(txt)
{
    box.set_text((char* )"Hello world! I am ""\xFF\x01\xf0\x00\x00Howard\xFF\x01\xff\xff\x00"".\n"
                    "\xFF\x81\x78\x00\x00"
                    "\xFF\x81\x0a\x00\x00"
                    "\xFF\x01\xf0\x00\x00""Howard""\xFF\x01\xff\xff\x00"" the Psyduck!\n"
                    "How goes? I'm fine, headache-y but fine.\n"
                    "I wonder... Heh, Let's see if it works correctly, shall we?");
    WalrusRPG::Animation p;
    p.stripe.push_back({0, 10});
    p.stripe.push_back({1, 10});
    anim.add_animation(0, p);
}

void StateMap::update(unsigned dt)
{
    if(!started){
        if(key_down(K_A))
            started = true;
        return;
    }
    unsigned t = dt*(key_down(K_B)?16:1);
    camera.update(t);
    anim.update(t);
    box.update(t);
}

void StateMap::render(unsigned dt)
{
    map.render(camera, dt);
    if(!started) return;
    box.render(dt);
    if(box.state == WalrusRPG::TextboxState::Updating)
        Graphics::put_sprite(tex_psyduck, 4, 4, {32*static_cast<signed>(anim.get_animation_frame(0)), 0, 32, 32});
    else
        Graphics::put_sprite(tex_psyduck, 4, 4, {0, 0, 32, 32});
    // print_debug_camera_data(camera, txt);
    // print_debug_map_data(map, txt);
}

#include <cstdio>
#include <cstdarg>
#include <os.h>
#include "Timers.h"
#include "Graphics.h"
#include "Pixel.h"
#include "Map.h"
#include "Camera.h"
#include "Text.h"
#include "misc.h"
#include "sprites.h"
#include "version.h"
#include "Interrupts.h"

using namespace WalrusRPG;

void print_debug_camera_data(const Camera &camera)
{
    Graphics::Text::print_format(0, 8, "CAM : X : %d Y: %d", camera.get_x(), camera.get_y());
}

void print_debug_map_data(const Map &map)
{
    Graphics::Text::print_format(0, 16, "MAP : W: %d, H:%d", map.get_width(), map.get_height());
}

void map_loop(unsigned x, unsigned y, Map &map)
{
    // Free-running, no interrupts, divider = 1, 32 bit, wrapping
    Timers::mode(0, true, false, false, 1, true);
    Timers::load(0, 0);
    unsigned loop_time = 546; // 32768Hz/60ups
    unsigned loop_next = -loop_time;
    unsigned last_frame = 0;

    unsigned keep_running = 1;
    Camera camera((signed) x, (signed) y);

    // Tileset asdf(better_character, 9, 16);
    TileRenderer asdf(better_character, 9, 16);
    Entity test_char(115, 90, 9, 16, &asdf, 0);

    while (keep_running)
    {
        if (isKeyPressed(KEY_NSPIRE_ESC))
            keep_running = 0;

        camera.update(0);

        // Frameskip
        if (Timers::read(0) > loop_next)
        {
            Graphics::Pixel pix(Graphics::Green);
            // TODO?: Preset color macros/consts?
            Graphics::buffer_fill(pix);
            map.render(camera, 1);
            test_char.render(camera, 1);
            Graphics::Text::print_format(0, 0, "WalrusRPG test build %s", git_version);

            print_debug_camera_data(camera);
            print_debug_map_data(map);
            unsigned frame_stamp = Timers::read(0);
            Graphics::Text::print_format(0, 240 - 8, "%u fps", 32768 / (last_frame - frame_stamp));
            last_frame = frame_stamp;
            Graphics::buffer_swap_render();
        }

        // Frame limiting
        while (Timers::read(0) > loop_next)
            ;
        loop_next -= loop_time;
    }
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    Graphics::buffer_allocate();
    Timers::init(0);
    Interrupts::init();

    uint16_t dungeonTest[] = {
        21, 21,  1,   1,   1,   1,   21,  22,  21,  22, 21,  22,  21,  21,  1,   22,  21,
        1,  22,  22,  22,  1,   21,  2,   3,   3,   3,  3,   3,   4,   21,  1,   22,  21,
        22, 22,  21,  21,  21,  1,   22,  22,  22,  23, 108, 109, 109, 109, 24,  87,  4,
        21, 21,  22,  5,   6,   6,   7,   1,   1,   22, 21,  21,  23,  66,  67,  108, 109,
        24, 109, 25,  21,  22,  5,   132, 43,  43,  28, 1,   1,   21,  22,  1,   23,  25,
        23, 109, 109, 108, 108, 25,  1,   1,   26,  42, 110, 48,  49,  22,  21,  1,   21,
        21, 23,  87,  88,  109, 24,  109, 109, 25,  1,  1,   26,  43,  131, 6,   7,   21,
        1,  22,  1,   21,  44,  67,  109, 24,  24,  24, 66,  46,  1,   22,  26,  27,  43,
        42, 131, 7,   21,  22,  1,   22,  21,  44,  45, 45,  45,  45,  46,  1,   22,  1,
        26, 27,  27,  43,  27,  28,  22,  21,  22,  21, 1,   21,  1,   22,  22,  21,  1,
        21, 22,  1,   47,  48,  111, 42,  27,  28,  21, 21,  1,   21,  21,  22,  2,   3,
        3,  4,   1,   2,   3,   4,   1,   5,   132, 27, 27,  28,  1,   1,   22,  1,   22,
        21, 23,  24,  66,  46,  1,   23,  24,  25,  1,  26,  42,  42,  110, 49,  21,  22,
        21, 22,  22,  2,   88,  24,  25,  2,   3,   88, 24,  87,  4,   26,  43,  110, 49,
        21, 21,  1,   1,   1,   2,   88,  24,  24,  87, 88,  108, 24,  24,  109, 25,  47,
        48, 49,  1,   22,  22,  21,  1,   21,  23,  24, 24,  24,  24,  24,  24,  24,  24,
        24, 87,  4,   21,  21,  22,  22,  22,  22,  1,  21,  23,  24,  24,  109, 24,  24,
        24, 24,  24,  108, 109, 25,  21,  21,  22,  22, 22,  22,  21,  21,  44,  45,  45,
        67, 24,  24,  24,  66,  45,  45,  45,  46,  1,  22,  1,   22,  22,  22,  21,  22,
        22, 22,  1,   44,  67,  108, 108, 25,  22,  22, 1,   22,  21,  22,  21,  21,  1,
        21, 22,  1,   22,  22,  1,   22,  44,  45,  45, 46,  1,   1,   1,   1,   21,  21,
        21, 21,  21,  21,  22,  21,  21,  21,  1,   21, 1,   22,  22,  22,  1,   21,  22,
        21, 1,   1,   22,  21,  1,   1,   21,  1,   1,  21,  21,  21,  1,   22,  22,  1,
        21, 22,  21,  22,  1,   22,  21,  21,  21,
    };
    uint16_t dungeonTest2[] = {
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   52,  53,  54,  55,  0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   73,  74,  75,  76,  0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   157, 158, 140, 160, 0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   178, 179, 161, 181, 0,  12, 14, 12, 14, 162, 163, 164, 0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  183, 184, 185, 0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  166, 0,   0,   186, 0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   145, 167, 0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 133, 134, 134, 135, 0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 154, 155, 155, 71,  135, 39, 40, 41, 0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 154, 155, 155, 155, 156, 60, 61, 62, 0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 175, 176, 51,  155, 156, 81, 82, 83, 0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   0,   175, 176, 177, 0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
        0, 0, 0, 0, 0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,   0,   0,   0,   0, 0,
    };

    Map map(20, 20, dungeonTest, dungeonTest2);
    tinystl::vector<Frame> stripe21;
    tinystl::vector<Frame> stripe22;
    stripe21.push_back({21, 23});
    stripe21.push_back({22, 31});
    stripe22.push_back({22, 37});
    stripe22.push_back({21, 41});
    map.anim.add_animation(21, {stripe21, true});
    map.anim.add_animation(22, {stripe22, true});
    map_loop(0, 0, map);

    Interrupts::off();
    Timers::restore(0);
    Graphics::buffer_free();
    return 0;
}

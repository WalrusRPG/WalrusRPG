#include "StateMachine.h"
#include "Timing.h"
#include "platform.h"
#include "Graphics.h"
#include "render/Text.h"
#include "version.h"
#include "input/Input.h"

using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::States;
using namespace WalrusRPG::Timing;
using WalrusRPG::Input::Key;
using WalrusRPG::Input::KeyState;

#define STATEMACHINE WalrusRPG::StateMachine

namespace
{
    void draw_button(unsigned x, unsigned y, KeyState state)
    {
        put_horizontal_line(x + 1, x + 5, y, Gray);
        put_horizontal_line(x + 1, x + 5, y + 6, Gray);
        put_vertical_line(x, y + 1, y + 5, Gray);
        put_vertical_line(x + 6, y + 1, y + 5, Gray);
        switch (state)
        {
            case KeyState::KS_RELEASED:
                put_rectangle({x + 1, y + 1, 5, 5}, White);
                break;
            case KeyState::KS_JUST_RELEASED:
                put_rectangle({x + 1, y + 1, 5, 5}, Magenta);
                break;
            case KeyState::KS_JUST_PRESSED:
                put_rectangle({x + 1, y + 1, 5, 5}, Cyan);
                break;
            case KeyState::KS_PRESSED:
                put_rectangle({x + 1, y + 1, 5, 5}, Black);
                break;
        }
    }
    void draw_buttons()
    {
        draw_button(0, 24, key_get_state(Key::K_L));
        draw_button(56, 24, key_get_state(Key::K_R));

        draw_button(8, 32, key_get_state(Key::K_UP));
        draw_button(0, 40, key_get_state(Key::K_LEFT));
        draw_button(16, 40, key_get_state(Key::K_RIGHT));
        draw_button(8, 48, key_get_state(Key::K_DOWN));

        draw_button(28, 48, key_get_state(Key::K_SELECT));
        draw_button(36, 48, key_get_state(Key::K_START));

        draw_button(48, 44, key_get_state(Key::K_B));
        draw_button(56, 36, key_get_state(Key::K_A));
    }
} /* namespace  */

STATEMACHINE::StateMachine(State *state)
{
    push(state);
}

STATEMACHINE::~StateMachine()
{
    while (!stack.empty())
        pop();
}

void STATEMACHINE::push(State *state)
{
    stack.push_back(state);
}

void STATEMACHINE::pop()
{
    delete stack.back();
    stack.pop_back();
}

void STATEMACHINE::run()
{
    const unsigned loop_time = TIMER_FREQ / 60;
    unsigned loop_next = loop_time;
    unsigned last_update = 0, update_stamp, update_time;
    unsigned last_frame = 0, frame_stamp, frame_time;

    while (!stack.empty())
    {
        update_stamp = Timing::gettime();
        update_time = update_stamp - last_update;
        Input::key_poll();
        stack.back()->update(100 * update_time / TIMER_FREQ);
        last_update = update_stamp;

        if (Timing::gettime() < loop_next)
        {
            frame_stamp = Timing::gettime();
            frame_time = frame_stamp - last_frame;
            Graphics::frame_begin();
            stack.back()->render(100 * frame_time / TIMER_FREQ);
            last_frame = frame_stamp;

            // Text::print_format(0, 0, "WalrusRPG test build %s", git_version);
            // if (frame_time != 0 && update_time != 0)
            // {
            //     Text::print_format(0, 240 - 8, "%ufps, %uups", TIMER_FREQ / frame_time,
            //                        TIMER_FREQ / update_time);
            // }
            // draw_buttons();
            Graphics::frame_end();
        }

        if (Input::key_pressed(Key::K_SELECT))
        {
            while (Input::key_down(Key::K_SELECT))
                Input::key_poll();
            this->pop();
        }

#ifdef ACTIVE_WAIT
        while (Timing::gettime() < loop_next)
            ;
#endif
        loop_next += loop_time;
    }
}

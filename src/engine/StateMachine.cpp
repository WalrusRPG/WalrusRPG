#include "StateMachine.h"
#include "Timing.h"
#include "platform.h"
#include "Graphics.h"
#include "render/Text.h"
#include "version.h"
#include "Input.h"

using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::States;
using namespace WalrusRPG::Timing;
using WalrusRPG::Input::Key;

#define STATEMACHINE WalrusRPG::StateMachine

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
        stack.back()->update(100*update_time/TIMER_FREQ);
        last_update = update_stamp;

        if (Timing::gettime() < loop_next)
        {
            frame_stamp = Timing::gettime();
            frame_time = frame_stamp - last_frame;
            Graphics::frame_begin();
            stack.back()->render(100*frame_time/TIMER_FREQ);
            last_frame = frame_stamp;

            Text::print_format(0, 0, "WalrusRPG test build %s", git_version);
            if (frame_time != 0 && update_time != 0)
            {
                Text::print_format(0, 240 - 8, "%ufps, %uups", TIMER_FREQ / frame_time,
                                   TIMER_FREQ / update_time);
            }
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

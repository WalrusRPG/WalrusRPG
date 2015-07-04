#include "StateMachine.h"
#include "Timers.h"
#include "Graphics.h"
#include "Text.h"
#include "version.h"
#include <os.h>

#define STATEMACHINE WalrusRPG::StateMachine

STATEMACHINE::StateMachine(WalrusRPG::States::State *state)
{
    push(state);
}

STATEMACHINE::~StateMachine()
{
}

void STATEMACHINE::push(WalrusRPG::States::State *state)
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
    Timers::mode(0, true, false, false, 1, true);
    Timers::load(0, 0);
    const unsigned loop_time = 546; // 32768Hz/60
    unsigned loop_next = -loop_time;
    unsigned last_frame = 0;

    while (!stack.empty())
    {
        stack.back()->update(loop_time);

        if (Timers::read(0) > loop_next)
        {
            stack.back()->render(loop_time);
            Graphics::Text::print_format(0, 0, "WalrusRPG test build %s", git_version);
            unsigned frame_stamp = Timers::read(0);
            Graphics::Text::print_format(0, 240 - 8, "%u fps",
                                         32768 / (last_frame - frame_stamp));
            last_frame = frame_stamp;
            Graphics::buffer_swap_render();
        }

        if (isKeyPressed(KEY_NSPIRE_ESC))
            this->pop();

        while (Timers::read(0) > loop_next)
            ;
        loop_next -= loop_time;
    }
}

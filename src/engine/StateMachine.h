#ifndef INCLUDE_STATEMACHINE_H
#define INCLUDE_STATEMACHINE_H

#include <TINYSTL/vector.h>
#include "State.h"

namespace WalrusRPG
{
    // StateMachine is the core system of WRPG.
    // It stores the main game states and update the current one.
    namespace StateMachine
    {
        void init();
        void deinit();
        void push(WalrusRPG::States::State *state);
        void pop();
        /**
         * The main Update/Render loop of the engine.
         */
        void run();
    };
}

#endif

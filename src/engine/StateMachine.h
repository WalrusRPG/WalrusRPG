#ifndef INCLUDE_STATEMACHINE_H
#define INCLUDE_STATEMACHINE_H

#include <TINYSTL/vector.h>
#include "State.h"

namespace WalrusRPG
{
    namespace StateMachine
    {
        void init();
        void deinit();
        void push(WalrusRPG::States::State *state);
        void pop();
        void run();
    };
}

#endif

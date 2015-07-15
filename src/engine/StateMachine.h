#ifndef INCLUDE_STATEMACHINE_H
#define INCLUDE_STATEMACHINE_H

#include <TINYSTL/vector.h>
#include "State.h"

namespace WalrusRPG
{
    class StateMachine
    {
      protected:
        tinystl::vector<WalrusRPG::States::State *> stack;

      public:
        StateMachine(WalrusRPG::States::State *state);
        ~StateMachine();
        void push(WalrusRPG::States::State *state);
        void pop();
        void run();
    };
}

#endif

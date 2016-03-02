#ifndef INCLUDE_STATE_H
#define INCLUDE_STATE_H

#if TARGET_SFML
#include "imgui.h"
#include "imgui-events-SFML.h"
#include "imgui-rendering-SFML.h"
#endif

namespace WalrusRPG
{
    namespace States
    {
        class State
        {
          public:
            virtual ~State(){};
            virtual void update(unsigned dt) = 0;
            virtual void render(unsigned dt) = 0;
#if TARGET_SFML
            virtual void debug(unsigned dt) = 0;
#endif
        };
    }
}


#endif

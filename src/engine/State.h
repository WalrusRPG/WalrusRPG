#ifndef INCLUDE_STATE_H
#define INCLUDE_STATE_H

#if IMGUI
#include "imgui-events-SFML.h"
#include "imgui-rendering-SFML.h"
#include "imgui.h"
#endif

namespace WalrusRPG
{
    namespace States
    {
        class State
        {
          public:
            virtual ~State(){};
            virtual void update() = 0;
            virtual void render() = 0;
#if IMGUI
            virtual void debug(){};
#endif
        };
    }
}


#endif

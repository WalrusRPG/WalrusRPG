#ifndef INCLUDE_STATE_H
#define INCLUDE_STATE_H

#include "Camera.h"

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
        };
    }
}


#endif

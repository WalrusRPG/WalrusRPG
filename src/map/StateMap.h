#ifndef INCLUDE_STATEMAP_H
#define INCLUDE_STATEMAP_H

#include "engine/State.h"
#include "Map.h"

namespace WalrusRPG
{
    namespace States
    {
        class StateMap : public State
        {
          protected:
            Camera camera;
            Map &map;

          public:
            StateMap(int x, int y, Map &map);
            void render(unsigned dt);
            void update(unsigned dt);
        };
    }
}
#endif

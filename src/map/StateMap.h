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
#if TARGET_SFML
            unsigned active_map_mode;
#endif
          public:
            StateMap(int x, int y, Map &map);
            void render(unsigned dt);
            void update(unsigned dt);
#if TARGET_SFML
            void debug(unsigned dt);
#endif
        };
    }
}
#endif

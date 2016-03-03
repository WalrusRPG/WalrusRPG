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
            void draw_background_tile(unsigned mode, float x, float y, float x2, float y2, unsigned tile);
            void draw_front_tile(unsigned mode, float x, float y, float x2, float y2, unsigned tile);

            void debug(unsigned dt);
#endif
        };
    }
}
#endif

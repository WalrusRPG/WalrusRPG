#ifndef INCLUDE_STATECOLLISION_H
#define INCLUDE_STATECOLLISION_H

#include "render/Camera.h"
#include "State.h"
#include "utility/Rect.h"

namespace WalrusRPG
{
    struct Box
    {
        Box(float _x, float _y, float _w, float _h, float _vx, float _vy)
        {
            x = _x;
            y = _y;
            w = _w;
            h = _h;
            vx = _vx;
            vy = _vy;
        }

        Box(float _x, float _y, float _w, float _h)
        {
            x = _x;
            y = _y;
            w = _w;
            h = _h;
            vx = 0.0f;
            vy = 0.0f;
        }

        // position of top-left corner
        float x, y;

        // dimensions
        float w, h;

        // velocity
        float vx, vy;
    };

    namespace States
    {
        class StateCollision : public State
        {
          private:
            bool collided_top;
            bool collided_bottom;
            bool collided_left;
            bool collided_right;
            WalrusRPG::Box p;
            WalrusRPG::Box target;
            char map[20][20];
            WalrusRPG::Camera cam;

          public:
            StateCollision();
            ~StateCollision();
            void update(unsigned dt) override;
            void render(unsigned dt) override;
        };
    }
}
#endif
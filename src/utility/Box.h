#ifndef INCLUDE_BOX_X
#define INCLUDE_BOX_X

namespace WalrusRPG
{
    /// TODO : Move this into an entity.
    struct Box
    {
        Box(float _x, float _y, unsigned _w, unsigned _h, float _vx, float _vy)
        {
            x = _x;
            y = _y;
            w = _w;
            h = _h;
            vx = _vx;
            vy = _vy;
        }

        Box(float _x, float _y, unsigned _w, unsigned _h)
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
        unsigned w, h;

        // velocity
        float vx, vy;
    };
}

#endif

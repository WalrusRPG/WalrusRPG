#include "Collision.h"

using WalrusRPG::Utils::Rect;

// returns true if the boxes are colliding (velocities are not used)
bool WalrusRPG::AABBCheck(Rect b1, Rect b2)
{
    return !(b1.x + b1.width <= b2.x || b1.x >= b2.x + b2.width ||
             b1.y + b1.height <= b2.y || b1.y >= b2.y + b2.height);
}

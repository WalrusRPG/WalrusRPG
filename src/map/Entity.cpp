#include "Entity.h"
#include "utility/misc.h"
#include "utility/Rect.h"

using WalrusRPG::Entity;
using namespace WalrusRPG::Utils;

Entity::Entity(float x, float y, unsigned w, unsigned h, WalrusRPG::Renderer *tset,
               unsigned sprite_id)
    : x(x), y(y), w(w), h(h), tset(tset), sprite_id(sprite_id), moving(false), solid(true)
{
}

Entity::~Entity()
{
    delete tset;
}

void Entity::render(Camera &camera, unsigned dt) const
{
    UNUSED(dt);
    if (tset == nullptr)
        return;
    if (camera.is_visible({(int) x, (int) y, w, h}))
    {
        tset->render(sprite_id, Rect(x - camera.get_x(), y - camera.get_y()));
        //(*tset).render_tile(sprite_id, coords.x - camera.get_x(), coords.y -
        // camera.get_y(), dt);
    }
}

void Entity::update(unsigned dt)
{
    UNUSED(dt);
    // TODO update map's data according to elasped time
    /*
                // Need to think aagain on how to go to a target point and/or we need to
       align the corner OR the center to this point.
                position += velocity * dt;
                velocity += acceleration * dt;
         */
}

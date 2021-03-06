#include "Entity.h"
#include "StateMap.h"
#include "utility/Rect.h"
#include "utility/misc.h"

using WalrusRPG::Entity;
using namespace WalrusRPG::Utils;

Entity::Entity(States::StateMap &container, float x, float y, unsigned w, unsigned h,
               WalrusRPG::Renderer *tset, unsigned sprite_id)
    : sprite_id(sprite_id), container(container), tset(tset), x(x), y(y), w(w), h(h),
      moving(false), solid(true)
{
}

Entity::~Entity()
{
    delete tset;
}

void Entity::render(Camera &camera) const
{
    if (tset == nullptr)
        return;
    if (camera.is_visible({(int) x, (int) y, w, h}))
    {
        tset->render(sprite_id, Rect(x - camera.get_x(), y - camera.get_y()));
        //(*tset).render_tile(sprite_id, coords.x - camera.get_x(), coords.y -
        // camera.get_y(), dt);
    }
}

void Entity::update()
{
    // TODO update map's data according to elasped time
    /*
                // Need to think aagain on how to go to a target point and/or we need to
       align the corner OR the center to this point.
                position += velocity * dt;
                velocity += acceleration * dt;
         */
}

void Entity::interact_with(Entity &origin, InteractionType type)
{
    container.box.set_text("Undefined behaviour.");
    container.started = true;

    UNUSED(origin);
    UNUSED(type);
}
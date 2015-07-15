#include "Entity.h"
#include "../utility/misc.h"
#include "../utility/Rect.h"
#include "../render/Text.h"

#define ENTITY WalrusRPG::Entity
using namespace WalrusRPG::Utils;

ENTITY::Entity(int x, int y, unsigned w, unsigned h, WalrusRPG::Renderer *tset,
               unsigned sprite_id)
    : coords(x, y, w, h), tset(tset), sprite_id(sprite_id)
{
}

ENTITY::~Entity()
{
    // TODO if you allocate dynamically members
}

void ENTITY::render(Camera &camera, unsigned dt) const
{
    UNUSED(dt);

    if (camera.is_visible(coords))
    {
        tset->render(sprite_id,
                     Rect(coords.x - camera.get_x(), coords.y - camera.get_y()));
        //(*tset).render_tile(sprite_id, coords.x - camera.get_x(), coords.y -
        // camera.get_y(), dt);
    }
}

void ENTITY::update(unsigned dt)
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

#include "Entity.h"
#include "misc.h"

#define ENTITY WalrusRPG::Entity

ENTITY::Entity()
{
    // TODO
}

ENTITY::~Entity()
{
    // TODO if you allocate dynamically members
}

void ENTITY::update(unsigned dt)
{
    UNUSED(dt);
    // TODO update map's data according to elasped time
    /*
		// Need to think aagain on how to go to a target point and/or we need to align the corner OR the center to this point.
		position += velocity * dt;
		velocity += acceleration * dt;
	 */
}

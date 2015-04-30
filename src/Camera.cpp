#include <os.h>
#include "Camera.h"
#include "Entity.h"
#include "misc.h"

#define CAMERA WalrusRPG::Camera

CAMERA::Camera(unsigned x, unsigned y)
{
	this->x = x;
	this->y = y;
}

CAMERA::~Camera()
{
	// TODO if you allocate dynamically members
}

void CAMERA::update(unsigned dt)
{
	UNUSED(dt);
	// TODO update map's data according to elasped time
	/*
		// Need to think aagain on how to go to a target point and/or we need to align the corner OR the center to this point.
		position += velocity * dt;
		velocity += acceleration * dt;
	 */

	if (isKeyPressed(KEY_NSPIRE_5)) this->y++;
	if (isKeyPressed(KEY_NSPIRE_8)) this->y--;
	if (isKeyPressed(KEY_NSPIRE_6)) this->x++;
	if (isKeyPressed(KEY_NSPIRE_4)) this->x--;
}


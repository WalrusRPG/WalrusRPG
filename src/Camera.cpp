#include <os.h>
#include "Camera.h"
#include "Entity.h"
#include "misc.h"

#define CAMERA WalrusRPG::Camera

CAMERA::Camera(signed x, signed y)
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

	if (isKeyPressed(KEY_NSPIRE_5)) y++;
	if (isKeyPressed(KEY_NSPIRE_8)) y--;
	if (isKeyPressed(KEY_NSPIRE_6)) x++;
	if (isKeyPressed(KEY_NSPIRE_4)) x--;
}

void CAMERA::set_x(signed x)
{
	this->x = x;
}

signed CAMERA::get_x() const
{
	return this->x;
}

void CAMERA::set_y(signed y)
{
	this->y = y;
}

signed CAMERA::get_y() const
{
	return this->y;
}

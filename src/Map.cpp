#include "Map.h"
#include "Camera.h"
#include "Graphics.h"
#include "sprites.h"
#include "misc.h"

#define MAP WalrusRPG::Map

MAP::Map(int width, int height, unsigned *layer0, unsigned *layer1)
{
	this->width = width;
	this->height = height;
	this->layer0 = layer0;
	this->layer1 = layer1;
}

MAP::~Map()
{
	// TODO if you allocate dynamically members
}

void MAP::update(unsigned dt)
{
	UNUSED(dt);
	// TODO update map's data according to elasped time
}

void MAP::render(WalrusRPG::Camera &camera, unsigned dt) const
{
	UNUSED(dt);
	unsigned offset_x = camera.get_x() % 24 * -1;
	unsigned offset_y = camera.get_y() % 24 * -1;

	WalrusRPG::Graphics::Rect_t sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	for (unsigned j = 0; j < 11; j++)
	{
		for (unsigned i = 0; i < 15; i++)
		{
			sprite.x = this->layer0[(camera.get_x() / 24 + i) + (camera.get_y() / 24 + j) * this->width] * 24;
			draw_sprite_sheet(tiles, offset_x + i * 24, offset_y + j * 24, &sprite);
		}
	}
}

bool MAP::entity_collide(Entity &entity) const
{
	UNUSED(entity);
	return false;
}

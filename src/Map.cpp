#include "Map.h"
#include "Camera.h"
#include "graphics.h"
#include "sprites.h"

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

void MAP::update(float dt)
{
	// TODO update map's data according to elasped time
}

void MAP::render(WalrusRPG::Camera &camera, float dt) const
{
	unsigned offset_x = camera.x % 24 * -1;
	unsigned offset_y = camera.y % 24 * -1;

	Rect_t sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	for (unsigned j = 0; j < 11; j++)
	{
		for (unsigned i = 0; i < 15; i++)
		{
			sprite.x = this->layer0[(camera.x / 24 + i) + (camera.y / 24 + j) * this->width] * 24;
			draw_sprite_sheet(tiles, offset_x + i * 24, offset_y + j * 24, &sprite);
		}		
	}
}

bool MAP::entity_collide(Entity &entity) const
{
	return false;
}


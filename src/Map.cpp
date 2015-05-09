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
	// By Eiyeron : I assumed that the camera's position is the top left pixel.
	// Margins moves the rendered map if we go outside of the bounds.
	signed margin_x = 0, margin_y = 0;
	signed offset_x = camera.get_x() % 24 * -1;
	signed offset_y = camera.get_y() % 24 * -1;

	signed start_x = camera.get_x() / 24;
	signed start_y = camera.get_y() / 24;
	signed end_x = start_x + 15;
	signed end_y = start_y + 16;

	if(start_x < 0){
		margin_x = -start_x*24;
		start_x = 0;
		}

	if(start_y < 0) {
		margin_y = -start_y*24;
		start_y = 0;
	}

	if(end_x > this->width) end_x = this->width;
	if(end_y > this->height) end_y = this->height;

	WalrusRPG::Graphics::Rect_t sprite;
	sprite.y = 0;
	sprite.w = 24;
	sprite.h = 24;

	for (unsigned j = 0; j < end_y - start_y; j++)
	{
		for (unsigned i = 0; i < end_x - start_x; i++)
		{
			unsigned index = (start_x + i) + (start_y + j) * this->width;
			sprite.x = this->layer0[index] * 24;
			draw_sprite_sheet(tiles, margin_x + offset_x + i * 24, margin_y + offset_y + j * 24, &sprite);
		}
	}

}

bool MAP::entity_collide(Entity &entity) const
{
	UNUSED(entity);
	return false;
}

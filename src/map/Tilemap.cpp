#include "Tilemap.h"

using WalrusRPG::Tilemap;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Utils::Rect;
using namespace WalrusRPG;

Tilemap::Tilemap(WalrusRPG::Graphics::Texture &tex, uint8_t*collision_masks, uint16_t nb_tiles)
	: tex(tex), nb_tiles(nb_tiles), collision_masks(collision_masks)
{
	nb_tile_width = tex.get_dimensions().width / TILE_DIMENSION;
	nb_tile_height = tex.get_dimensions().height / TILE_DIMENSION;
}

Tilemap::~Tilemap()
{

}

const Texture& Tilemap::get_texture() const
{
	return tex;
}

Rect Tilemap::get_tile(uint16_t id) const
{
	return {(id%nb_tile_width)*TILE_DIMENSION, (id/nb_tile_width)*TILE_DIMENSION, TILE_DIMENSION, TILE_DIMENSION};
}

uint8_t Tilemap::get_collision(uint16_t id) const
{
	if(id >= nb_tiles)
		return 0;
	return collision_masks[id];
}

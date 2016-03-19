#include "Tilemap.h"
#include "Graphics.h"

using WalrusRPG::Tilemap;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Utils::Rect;
using namespace WalrusRPG;
using namespace WalrusRPG::Graphics;

Tilemap::Tilemap(WalrusRPG::Graphics::Texture &tex, uint8_t *collision_masks,
                 uint16_t nb_tiles)
    : anim(), tex(tex), nb_tiles(nb_tiles), collision_masks(collision_masks)
{
    nb_tile_width = tex.get_dimensions().width / TILE_DIMENSION;
    nb_tile_height = tex.get_dimensions().height / TILE_DIMENSION;
}

Tilemap::~Tilemap()
{
}

const Texture &Tilemap::get_texture() const
{
    return tex;
}

Rect Tilemap::get_tile(uint16_t id) const
{
    return {(id % nb_tile_width) * TILE_DIMENSION, (id / nb_tile_width) * TILE_DIMENSION,
            TILE_DIMENSION, TILE_DIMENSION};
}

uint8_t Tilemap::get_collision(uint16_t id) const
{
    if (id >= nb_tiles)
        return 0;
    return collision_masks[id];
}

void Tilemap::render_tile(uint16_t id, int x, int y) const
{
    Graphics::put_sprite(tex, x, y, get_tile(anim.get_animation_frame(id)));
}

void Tilemap::render_collision_mask(uint16_t id, int x, int y) const
{
    if(id >= nb_tiles) return;
    const int half_dim = TILE_DIMENSION/2;
    uint8_t mask = collision_masks[id];
    if(mask & TOP_LEFT_CORNER)
        Graphics::put_rectangle({x,y,half_dim, half_dim}, White);
    if(mask & TOP_RIGHT_CORNER)
        Graphics::put_rectangle({x+half_dim,y,half_dim, half_dim}, White);
    if(mask & BOTTOM_LEFT_CORNER)
        Graphics::put_rectangle({x,y+half_dim,half_dim, half_dim}, White);
    if(mask & BOTTOM_RIGHT_CORNER)
        Graphics::put_rectangle({x+half_dim,y+half_dim,half_dim, half_dim}, White);
}


#ifndef INCLUDE_TILEMAP_H
#define INCLUDE_TILEMAP_H

#include "Texture.h"
#include "utility/Rect.h"

namespace WalrusRPG
{
    /**
     This enum stores flags to enable square quadrants in a tile's hitbox.
     Note : Check collision branch to see why.
     -------
     |  |  |
     | 1|2 |
     |--|--|
     | 3|4 |
     |  |  |
     -------
    */
    enum TilemapCollisionPartsFlag
    {
        TOP_LEFT_CORNER = 1 << 0,
        TOP_RIGHT_CORNER = 1 << 1,
        BOTTOM_LEFT_CORNER = 1 << 2,
        BOTTOM_RIGHT_CORNER = 1 << 3,
    };

    class Tilemap
    {
      public:
        constexpr static int TILE_DIMENSION = 16;

      protected:
        WalrusRPG::Graphics::Texture tex;
        // Will be passed along the collision mask list.
        uint16_t nb_tiles;
        // Precalculated for faster access.
        uint16_t nb_tile_width, nb_tile_height;
        uint8_t *collision_masks;


      public:
        // Tempoary generation for 
        Tilemap(WalrusRPG::Graphics::Texture &tex, uint8_t*collision_masks, uint16_t nb_tiles);
        ~Tilemap();
        const WalrusRPG::Graphics::Texture& get_texture() const;
        WalrusRPG::Utils::Rect get_tile(uint16_t id) const;
        uint8_t get_collision(uint16_t id) const;
    };
}
#endif
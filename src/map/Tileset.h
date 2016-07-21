#ifndef INCLUDE_TILEMAP_H
#define INCLUDE_TILEMAP_H

#include "Texture.h"
#include "piaf/Archive.h"
#include "render/Animator.h"
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
    enum TilesetCollisionPartsFlag
    {
        TOP_LEFT_CORNER = 0b0001,
        TOP_RIGHT_CORNER = 0b0010,
        BOTTOM_LEFT_CORNER = 0b0100,
        BOTTOM_RIGHT_CORNER = 0b1000,
    };

    struct Tilechip
    {
        uint8_t collision_mask;
    };

    constexpr const int TILE_DIMENSION = 16;
    class Tileset
    {
      public:
        constexpr static int TILE_HALF_DIMENSION = TILE_DIMENSION / 2;
        constexpr static int TILESET_HEADER_SIZE = 24;
        constexpr static int TILESET_CHIP_DATA_SIZE = 1;
        Animator anim;

      protected:
        WalrusRPG::Graphics::Texture tex;
        // Will be passed along the collision mask list.
        uint16_t nb_tiles;
        // Precalculated for faster access.
        uint16_t nb_tile_width, nb_tile_height;
        Tilechip *chips;


      public:
        // Tempoary generation for
        Tileset(WalrusRPG::Graphics::Texture &tex, uint8_t *collision_masks,
                uint16_t nb_tiles);
        Tileset(WalrusRPG::PIAF::File &tset_data, WalrusRPG::PIAF::File &tex_file);
        // Small thing to allow Map to init it directly from its consturctor.
        Tileset(WalrusRPG::PIAF::File &&tset_data, WalrusRPG::PIAF::File &&tex_file);
        ~Tileset();

        WalrusRPG::Graphics::Texture &get_texture();
        WalrusRPG::Utils::Rect get_tile(uint16_t id) const;
        uint8_t get_collision(uint16_t id) const;
        void render_tile(uint16_t id, int x, int y) const;
        void render_collision_mask(uint16_t id, int x, int y) const;
    };

    class TilesetException : public std::exception
    {
      private:
        char msg[1024];

      public:
        TilesetException(const char *format, ...);
        virtual ~TilesetException();

        const char *what() const throw();
    };
}
#endif

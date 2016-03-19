#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include <stdint.h>
#include "Texture.h"
#include "Tilemap.h"
#include "render/Camera.h"
#include "render/TileRenderer.h"

namespace WalrusRPG
{
    class Map
    {
      protected:
        // <Tiles> data;
        // <Tileset> tileset;
        int width;
        int height;
        uint16_t *layer0;
        uint16_t *layer1;
        // TODO?: add a boolean/getter to know if a second layer exist?
      public:
        Tilemap tmap;

        Map(int width, int height, uint16_t *layer0, uint16_t *layer1,
            WalrusRPG::Graphics::Texture &tex);
        ~Map();
        void render(Camera &camera, unsigned dt);
        void update(unsigned dt);
        bool is_tile_solid(int x, int y) const;
        bool is_pixel_solid(int x, int y) const;
        int get_width() const;
        int get_height() const;
    };
}

#endif

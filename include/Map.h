#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include "Camera.h"
#include "Entity.h"
#include "Tileset.h"

namespace WalrusRPG
{
    class Map
    {
      protected:
        // <Tiles> data;
        // <Tileset> tileset;
        unsigned int width;
        unsigned int height;
        unsigned *layer0;
        unsigned *layer1;
        Tileset tset;
        unsigned time_render;
        // TODO?: add a boolean/getter to know if a second layer exist?
      public:
        Map(int width, int height, unsigned *layer0, unsigned *layer1);
        ~Map();
        void render(Camera &camera, unsigned dt);
        void update(unsigned dt);
        bool is_tile_solid(unsigned x, unsigned y) const;
        bool is_pixel_solid(unsigned x, unsigned y) const;
        unsigned get_width() const;
        unsigned get_height() const;
    };
}

#endif

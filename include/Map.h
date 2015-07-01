#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include <stdint.h>
#include "Camera.h"
#include "Entity.h"
#include "TileRenderer.h"
#include "Animator.h"

namespace WalrusRPG
{
    class Map
    {
      public:
        Animator anim;

      protected:
        // <Tiles> data;
        // <Tileset> tileset;
        unsigned int width;
        unsigned int height;
        uint16_t *layer0;
        uint16_t *layer1;
        TileRenderer *renderer;
        // TODO?: add a boolean/getter to know if a second layer exist?
      public:
        Map(int width, int height, uint16_t *layer0, uint16_t *layer1);
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

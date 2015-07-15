#ifndef INCLUDE_TILERENDERER_H
#define INCLUDE_TILERENDERER_H

#include "../utility/Rect.h"
#include "Renderer.h"

namespace WalrusRPG
{
    class TileRenderer : public Renderer
    {
      protected:
        unsigned short *tilesheet;
        unsigned tile_width;
        unsigned tile_height;

      public:
        TileRenderer(unsigned short *tilesheet, unsigned tile_width,
                     unsigned tile_height);
        void render(const unsigned id, const WalrusRPG::Utils::Rect &rect);

        int get_tile_width() const;
        int get_tile_height() const;
        ~TileRenderer();
    };
}

#endif

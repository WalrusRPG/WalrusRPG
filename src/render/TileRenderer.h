#ifndef INCLUDE_TILERENDERER_H
#define INCLUDE_TILERENDERER_H

#include "Renderer.h"
#include "Texture.h"
#include "utility/Rect.h"

namespace WalrusRPG
{
    class TileRenderer : public Renderer
    {
      protected:
        WalrusRPG::Graphics::Texture tilesheet;
        unsigned tile_width;
        unsigned tile_height;

      public:
        TileRenderer(WalrusRPG::Graphics::Texture &tilesheet, unsigned tile_width,
                     unsigned tile_height);
        void render(const unsigned id, const WalrusRPG::Utils::Rect &rect);

        int get_tile_width() const;
        int get_tile_height() const;
        ~TileRenderer();
    };
}

#endif

#ifndef INCLUDE_SPRITERENDERER_H
#define INCLUDE_SPRITERENDERER_H

#include "Texture.h"
#include "render/Renderer.h"
#include "utility/Rect.h"
#include <TINYSTL/unordered_map.h>

namespace WalrusRPG
{
    class SpriteRenderer : public Renderer
    {
      protected:
        WalrusRPG::Graphics::Texture tilesheet;
        tinystl::unordered_map<unsigned, WalrusRPG::Utils::Rect> sprites;

      public:
        SpriteRenderer(WalrusRPG::Graphics::Texture tilesheet);
        void add_sprite(unsigned id, WalrusRPG::Utils::Rect rect);
        virtual void render(const unsigned id, const WalrusRPG::Utils::Rect &rect);
        void render(const unsigned id, const WalrusRPG::Utils::Rect &rect,
                    const WalrusRPG::Graphics::Pixel &tint);
    };
}

#endif

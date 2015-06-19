#ifndef INCLUDE_SPRITERENDERER_H
#define INCLUDE_SPRITERENDERER_H

#include <TINYSTL/unordered_map.h>
#include "Rect.h"
#include "Renderer.h"

namespace WalrusRPG
{
    class SpriteRenderer : public Renderer
    {
      protected:
        tinystl::unordered_map<unsigned, WalrusRPG::Uils::Rect> sprites;

      public:
        SpriteRenderer(/*char *tilesheet*/);
        virtual void render(const unsigned id, const WalrusRPG::Utils::Rect &rect) const throw;
    }
}

#endif

#ifndef INCLUDE_RENDERER_H
#define INCLUDE_RENDERER_H

#include "Rect.h"

namespace WalrusRPG
{
    class Renderer
    {
      public:
        virtual ~Renderer(){};
        virtual void render(const unsigned id, const WalrusRPG::Utils::Rect &rect) const = 0;
    };
}

#endif

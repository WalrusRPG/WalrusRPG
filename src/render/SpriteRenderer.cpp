#include <TINYSTL/unordered_map.h>
#include "SpriteRenderer.h"
#include "Graphics.h"

#define SPRITERENDERER WalrusRPG::SpriteRenderer
using namespace WalrusRPG;
using namespace WalrusRPG::Utils;

SPRITERENDERER::SpriteRenderer(unsigned short *_tilesheet) : tilesheet(_tilesheet)
{
}

void SPRITERENDERER::add_sprite(unsigned id, WalrusRPG::Utils::Rect rect)
{
    sprites[id] = rect;
}

void SPRITERENDERER::render(const unsigned id, const Rect &rect)
{
    Graphics::put_sprite(tilesheet, rect.x, rect.y, sprites[id]);
}

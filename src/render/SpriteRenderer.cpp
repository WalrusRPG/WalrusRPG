#include "SpriteRenderer.h"
#include "../drivers/Graphics.h"
#include "../utility/Rect.h"
#include <TINYSTL/unordered_map.h>

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
    Graphics::draw_sprite_sheet(tilesheet, rect.x, rect.y, sprites[id]);
}

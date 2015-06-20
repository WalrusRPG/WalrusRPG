#include "SpriteRenderer.h"
#include "Graphics.h"
#include "Rect.h"
#include <TINYSTL/unordered_map.h>

#define SPRITERENDERER WalrusRPG::SpriteRenderer
#define GRAPHICS WalrusRPG::Graphics
#define RECT WalrusRPG::Utils::Rect

SPRITERENDERER::SpriteRenderer(unsigned short *_tilesheet)
    : tilesheet(_tilesheet)
{
}

void SPRITERENDERER::add_sprite(unsigned id, WalrusRPG::Utils::Rect rect)
{
    sprites[id] = rect;
}

void SPRITERENDERER::render(const unsigned id, const RECT &rect)
{
    GRAPHICS::draw_sprite_sheet(tilesheet, rect.x, rect.y, sprites[id]);
}

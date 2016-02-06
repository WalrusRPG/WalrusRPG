#include <TINYSTL/unordered_map.h>
#include "SpriteRenderer.h"
#include "Graphics.h"
#include "render/Pixel.h"

using WalrusRPG::SpriteRenderer;
using namespace WalrusRPG;
using namespace WalrusRPG::Utils;
using WalrusRPG::Graphics::Pixel;

SpriteRenderer::SpriteRenderer(WalrusRPG::Graphics::Texture _tilesheet)
    : tilesheet(_tilesheet)
{
}

void SpriteRenderer::add_sprite(unsigned id, WalrusRPG::Utils::Rect rect)
{
    sprites[id] = rect;
}

void SpriteRenderer::render(const unsigned id, const Rect &rect)
{
    Graphics::put_sprite(tilesheet, rect.x, rect.y, sprites[id]);
}

void SpriteRenderer::render(const unsigned id, const Rect &rect, const Pixel &tint)
{
    Graphics::put_sprite_tint(tilesheet, rect.x, rect.y, sprites[id], tint);
}

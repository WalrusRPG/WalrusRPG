#include "TileRenderer.h"
#include "Graphics.h"

using WalrusRPG::TileRenderer;
using namespace WalrusRPG;
using namespace WalrusRPG::Utils;

TileRenderer::TileRenderer(WalrusRPG::Graphics::Texture &_tilesheet, unsigned tile_width,
                           unsigned tile_height)
    : tilesheet(_tilesheet), tile_width(tile_width), tile_height(tile_height)
{
}

void TileRenderer::render(const unsigned id, const Rect &rect)
{
    unsigned num_tiles_x = tilesheet.get_dimensions().width / tile_width;
    // unsigned num_tiles_y = sheet_height / tile_height;
    Graphics::put_sprite(tilesheet, rect.x, rect.y,
                         Rect(tile_width * (id % num_tiles_x),
                              tile_height * (id / num_tiles_x), tile_width, tile_height));
}

int TileRenderer::get_tile_width() const
{
    return tile_width;
}
int TileRenderer::get_tile_height() const
{
    return tile_height;
}


TileRenderer::~TileRenderer()
{
    // Nothing else than the current object is dynamically allocated, nothing should be
    // done here atm.
}

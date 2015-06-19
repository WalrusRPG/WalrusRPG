#include "TileRenderer.h"
#include "Graphics.h"
#include "Rect.h"

#define GRAPHICS WalrusRPG::Graphics
#define TILERENDERER WalrusRPG::TileRenderer
#define RECT WalrusRPG::Utils::Rect

TILERENDERER::TileRenderer(unsigned short *_tilesheet, unsigned tile_width, unsigned tile_height)
    : tilesheet(_tilesheet), tile_width(tile_width), tile_height(tile_height)
{
}

void TILERENDERER::render(const unsigned id, const RECT &rect) const
{
    unsigned num_tiles_x = tilesheet[0] / tile_width;
    //unsigned num_tiles_y = sheet_height / tile_height;
    GRAPHICS::draw_sprite_sheet(tilesheet, rect.x, rect.y, RECT(tile_width * (id % num_tiles_x), tile_height * (id / num_tiles_x), tile_width, tile_height));
}

int TILERENDERER::get_tile_width() const
{
    return tile_width;
}
int TILERENDERER::get_tile_height() const
{
    return tile_height;
}


TILERENDERER::~TileRenderer()
{
  // Nothing else than the current object is dynamically allocated, nothing should be done here atm.
}

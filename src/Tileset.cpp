#include "Tileset.h"
#include "Graphics.h"
#include "Rect.h"

#define GRAPHICS WalrusRPG::Graphics
#define TILESET WalrusRPG::Tileset
#define FRAME WalrusRPG::Frame
#define UTILS WalrusRPG::Utils

namespace
{
    unsigned find_frame(const tinystl::vector<WalrusRPG::Frame> &anim, signed frame_time)
    {
        unsigned index = 0;
        do
        {
            frame_time -= anim[index].duration;
            index = (index + 1) % anim.size();
        } while (frame_time > 0);
        return index;
    }
}

TILESET::Tileset(unsigned short *sheet, unsigned tile_width, unsigned tile_height)
    : sheet(sheet), tile_width(tile_width), tile_height(tile_height)
{
}

void TILESET::add_animation(int index, tinystl::vector<WalrusRPG::Frame> anim)
{
    animations[index] = anim;
}

void TILESET::render_tile(unsigned int index, unsigned x, unsigned y) const
{
    unsigned num_tiles_x = sheet[0] / tile_width;
    //unsigned num_tiles_y = sheet_height / tile_height;
    GRAPHICS::draw_sprite_sheet(sheet, x, y, UTILS::Rect(tile_width * (index % num_tiles_x), tile_height * (index / num_tiles_x), tile_width, tile_height));
}

void TILESET::render_tile(unsigned int index, unsigned x, unsigned y, unsigned time)
{
    if (!animations[index].empty())
    {
        render_tile(animations[index][find_frame(this->animations[index], time)].frame, x, y);
        // If an animation already exists.
    }
    else
        render_tile(index, x, y);
}

int TILESET::get_tile_width() const
{
    return tile_width;
}
int TILESET::get_tile_height() const
{
    return tile_height;
}

#include "Tileset.h"
#include "Graphics.h"

#define TILESET WalrusRPG::Tileset
#define FRAME WalrusRPG::Frame

namespace
{
    unsigned find_frame(const std::vector<WalrusRPG::Frame> &anim, signed frame_time)
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

TILESET::Tileset(unsigned short *sheet, unsigned sheet_width, unsigned sheet_height, unsigned tile_width, unsigned tile_height)
    : sheet(sheet), sheet_width(sheet_width), sheet_height(sheet_height), tile_width(tile_width), tile_height(tile_height)
{
}

void TILESET::add_animation(int index, std::vector<WalrusRPG::Frame> anim)
{
    animations[index] = anim;
}

void TILESET::render_tile(unsigned int index, unsigned x, unsigned y) const
{
    unsigned num_tiles_x = sheet_width / tile_width;
    //unsigned num_tiles_y = sheet_height / tile_height;

    WalrusRPG::Graphics::Rect_t sprite;
    sprite.w = tile_width;
    sprite.h = tile_height;
    sprite.x = tile_width * (index % num_tiles_x);
    sprite.y = tile_height * (index / num_tiles_x);
    draw_sprite_sheet(sheet, x, y, &sprite);
}

void TILESET::render_tile(unsigned int index, unsigned x, unsigned y, unsigned time) const
{
    if (animations.count(index))
    {
        render_tile(animations.at(index).at(find_frame(this->animations.at(index), time)).frame, x, y);
        // If an animation already exists.
    }
    else
        render_tile(index, x, y);
}

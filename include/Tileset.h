#ifndef INCLUDE_TILESET_H
#define INCLUDE_TILESET_H

#include <TINYSTL/vector.h>
#include <TINYSTL/unordered_map.h>

namespace WalrusRPG
{
    struct Frame
    {
        unsigned frame;
        unsigned duration;
    };
    class Tileset
    {
      protected:
        const unsigned short *sheet;
        unsigned sheet_width;
        unsigned sheet_height;
        unsigned tile_width;
        unsigned tile_height;
        tinystl::unordered_map<unsigned, tinystl::vector<Frame>> animations;

      public:
        Tileset(unsigned short *sheet, unsigned sheet_width, unsigned sheet_height, unsigned tile_width, unsigned tile_heihgt);
        void add_animation(int index, tinystl::vector<WalrusRPG::Frame> anim);
        void render_tile(unsigned int index, unsigned x, unsigned y) const;
        void render_tile(unsigned int index, unsigned x, unsigned y, unsigned time);

        int get_tile_width() const;
        int get_tile_height() const;
    };
}

#endif

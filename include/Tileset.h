#ifndef INCLUDE_TILESET_H
#define INCLUDE_TILESET_H

namespace WalrusRPG
{
    class Tileset
    {
      protected:
        char *sheet;
        unsigned sheet_width;
        unsigned sheet_height;
        unsigned tile_width;
        unsigned tile_heihgt;

      public:
        Tileset(char *sheet, unsigned sheet_width, unsigned sheet_height, unsigned tile_width, unsigned tile_heihgt);
        render_tile(unsigned int index, unsigned x, unsigned y) const;
    };
}

#endif

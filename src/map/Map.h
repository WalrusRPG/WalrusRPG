#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include <stdint.h>
#include "Texture.h"
#include "Tileset.h"
#include "piaf/Archive.h"
#include "render/Camera.h"
#include "render/TileRenderer.h"

namespace WalrusRPG
{
    enum MapCompression
    {
        UNKWOWN,
        RAW,
        RLE_PER_LAYER,
        RLE_ALL_LAYERS,
        ZLIB,
    };

    class Map
    {
      protected:
        uint16_t width;
        uint16_t height;
        uint16_t *layer0;
        uint16_t *layer1;
        // TODO?: add a boolean/getter to know if a second layer exist?
      public:
        Tileset tmap;

        Map(int width, int height, uint16_t *layer0, uint16_t *layer1,
            WalrusRPG::Graphics::Texture &tex);
        Map(WalrusRPG::PIAF::Archive &data_container, const char *map_filename,
            const char *tset_filename, const char *tex_filename);
        ~Map();
        void render(Camera &camera, unsigned dt);
        void update(unsigned dt);
        bool is_tile_solid(int x, int y) const;
        bool is_pixel_solid(int x, int y) const;
        int get_width() const;
        int get_height() const;
    };


    class MapException : public std::exception
    {
      private:
        char msg[1024];

      public:
        MapException(const char *format, ...);
        virtual ~MapException();

        const char *what() const throw();
    };
}

#endif

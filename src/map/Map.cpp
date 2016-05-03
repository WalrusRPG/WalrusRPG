#include "Map.h"
#include <cstdio>
#include <cstring>
#include <zlib.h>
#include "render/TileRenderer.h"
#include "Graphics.h"
#include "Logger.h"
#include "utility/Rect.h"
#include "utility/misc.h"
#include "piaf/Archive.h"

using WalrusRPG::Map;
using WalrusRPG::MapCompression;
using WalrusRPG::MapException;
using namespace WalrusRPG;
using namespace WalrusRPG::Utils;
using namespace WalrusRPG::PIAF;
using WalrusRPG::Graphics::Texture;

// Graphics::Texture tex_overworld((char *) overworld);
namespace
{
    void load_map(void *data, uint32_t datasize, uint16_t &map_width,
                  uint16_t &map_height, uint16_t *&layer0, uint16_t *&layer1)
    {
        char *cdata = (char *) data;
        uint32_t map_version;
        uint32_t expected_checksum, real_checksum;
        uint32_t expected_data_size, real_data_size;
        uint16_t map_n_layers = 0, map_n_events;
        enum MapCompression map_compression =
            MapCompression::RAW; // or whatever Map_Compression will be

        if (cdata == NULL)
        {
            Logger::error("Null pointer Exception");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Null pointer Exception", __FILE__);
#endif
        }
        if (datasize < 28)
        {
            Logger::error("File too small for header");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: File too small for header", __FILE__);
#endif
        }

        if (strncmp(cdata, "WMap", 4))
        {
            Logger::error("Bad map header");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Bad map header", __FILE__);
#endif
        }

        expected_checksum = read_big_endian_value<uint32_t>(&cdata[4]);
        real_checksum = crc32(0L, (unsigned char *) &cdata[8], 20);
        if (expected_checksum != real_checksum)
        {
            Logger::error("Bad chekcsum");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Bad chekcsum", __FILE__);
#endif
        }

        map_version = read_big_endian_value<uint32_t>(&cdata[8]);
        // TODO : version check

        expected_data_size = read_big_endian_value<uint32_t>(&cdata[12]);
        real_data_size = datasize - 28;
        if (expected_data_size != real_data_size)
        {
            Logger::error("Bad data size");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Bad data size", __FILE__);
#endif
        }

        map_width = read_big_endian_value<uint16_t>(&cdata[16]);
        map_height = read_big_endian_value<uint16_t>(&cdata[18]);

        map_n_layers = read_big_endian_value<uint16_t>(&cdata[20]);
        if (map_n_layers < 1 || map_n_layers > 2)
        {
            Logger::error("Wrong map layer number");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Wrong map layer number", __FILE__);
#endif
        }

        map_n_events = read_big_endian_value<uint16_t>(&cdata[22]);
        if (map_n_events != 0)
        {
            Logger::warn("Events not supported yet");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Events not supported yet", __FILE__);
#endif
        }

        map_compression = (MapCompression) read_big_endian_value<uint32_t>(&cdata[24]);

        layer0 = new uint16_t[map_width * map_height];
        layer1 = new uint16_t[map_width * map_height];
        switch (map_compression)
        {
            // copy layers from &udata[6] and
            // &udata[6*(map_width*map_height/sizeof(uint32_t))]
            case MapCompression::RAW:
                if (map_n_layers < 1)
                    break;
                for (unsigned i = 0, limit = map_width * map_height; i < limit; ++i)
                {
                    layer0[i] = read_big_endian_value<uint16_t>(
                        &cdata[28 + sizeof(uint16_t) * i]);
                }
                if (map_n_layers < 2)
                    break;
                for (unsigned i = 0, limit = map_width * map_height; i < limit; ++i)
                {
                    layer1[i] = read_big_endian_value<uint16_t>(
                        &cdata[28 + sizeof(uint16_t) * (map_width * map_height + i)]);
                }
                break;
            /* // NOT IMPLEMENTED YET //
            case RLE_PER_LAYER:
            // TODO : RLE support for map
            break;
            case RLE_ALL_LAYERS:
            // TODO
            break;
            case ZLIB:
            // TODO : ZLIB support for map
            break;
            */
            default:
                Logger::error("Wrong compression method");
#ifdef WRPG_EXCEPTIONS
                throw MapException("%s: Wrong compression method", __FILE__);
#endif
                break;
        }
    }
}


Map::Map(int width, int height, uint16_t *layer0, uint16_t *layer1, Texture &tex)
    : width(width), height(height), layer0(layer0), layer1(layer1), tmap(tex, nullptr, 0)
{
    Logger::warn("Deprecated");
}


Map::Map(Archive &data_container, const char *map_filename, const char *tset_filename,
         const char *tex_filename)
    : tmap(data_container.get(tset_filename), data_container.get(tex_filename))
{
    File map_file = data_container.get(map_filename);
    load_map((void *) map_file.get(), map_file.file_size, width, height, layer0, layer1);
}

Map::~Map()
{
}

void Map::update(unsigned dt)
{
    // TODO update map's data according to elasped time
    tmap.anim.update(dt);
}

void Map::render(WalrusRPG::Camera &camera, unsigned dt)
{
    UNUSED(dt);
    signed t_width = tmap.TILE_DIMENSION;
    signed t_height = tmap.TILE_DIMENSION;

    // Substractions here because we want to always round down when dividing
    signed offset_x = camera.get_x() % t_width * -1 - (camera.get_x() < 0) * t_width;
    signed offset_y = camera.get_y() % t_height * -1 - (camera.get_y() < 0) * t_height;
    signed start_x = camera.get_x() / t_width - (camera.get_x() < 0);
    signed start_y = camera.get_y() / t_height - (camera.get_y() < 0);

    // pre-calculating variables to speed up loop condition check
    signed delta_x = 320 / t_width + 1;
    signed delta_y = 240 / t_height + 1;

    // rendering part.
    Rect r = tmap.get_texture().get_dimensions();
    for (signed j = 0; j < delta_y; j++)
    {
        for (signed i = 0; i < delta_x; i++)
        {
            int index, tile_over;
            index = (start_x + i) + (start_y + j) * this->width;
            if (in_range(start_x + i, 0, (signed) width) &&
                in_range(start_y + j, 0, (signed) height))
                tile_over = this->layer0[index];
            else
                tile_over = 0;
            tmap.render_tile(tile_over, offset_x + i * t_width, offset_y + j * t_height);
            // tmap.render_collision_mask(tile_over, offset_x + i * t_width, offset_y + j
            // * t_height);

            // layer1 : Over-layer
            if (this->layer1 == nullptr)
                continue;
            if (in_range(start_x + i, 0, (signed) width) &&
                in_range(start_y + j, 0, (signed) height))
                tile_over = this->layer1[index];
            else
                tile_over = 0;
            if (tile_over != 0)
            {
                tmap.render_tile(tile_over, offset_x + i * t_width,
                                 offset_y + j * t_height);
                // tmap.render_collision_mask(tile_over, offset_x + i * t_width, offset_y
                // + j * t_height);
            }
        }
    }
}

bool Map::is_tile_solid(int x, int y) const
{
    if (x >= width || y >= height || x < 0 || y < 0)
        return true;
    return tmap.get_collision(this->layer0[y * width + x]) != 0;
}

bool Map::is_pixel_solid(int x, int y) const
{
    return is_tile_solid(x / tmap.TILE_DIMENSION, y / tmap.TILE_DIMENSION);
}

int Map::get_width() const
{
    return this->width;
}

int Map::get_height() const
{
    return this->width;
}

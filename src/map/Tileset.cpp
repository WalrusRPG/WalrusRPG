#include <cstring>
#include <zlib.h>
#include "Tileset.h"
#include "Logger.h"
#include "Graphics.h"
#include "utility/misc.h"

using WalrusRPG::Tileset;
using WalrusRPG::TilesetException;
using WalrusRPG::Graphics::Texture;
using WalrusRPG::Utils::Rect;
using WalrusRPG::PIAF::File;
using namespace WalrusRPG;
using namespace WalrusRPG::Graphics;

namespace
{
    void load_tileset(void *data, uint32_t datasize, uint16_t &nb_tiles, Tilechip *&chips)
    {
        char *cdata = (char *) data;

        uint32_t expected_checksum, real_checksum;
        uint32_t set_sprite_size, set_version;

        if (cdata == NULL)
        {
            Logger::error("WTst : NPE"); // Null Pointer Exception
#ifdef WRPG_EXCEPTIONS
            throw TilesetException("%s : Null Pointer Exception", __FILE__); // Null Pointer Exception
#endif
        }

        if (datasize < 24)
        {
            Logger::error("File too small for header.");
#ifdef WRPG_EXCEPTIONS
            throw TilesetException("%s, File too small for header.", __FILE__);
#endif
        }
        if (strncmp(cdata, "WTst", 4))
        {
            Logger::error("Bad tileset header. : %4s", cdata);
#ifdef WRPG_EXCEPTIONS
            throw TilesetException("%s, Bad tileset header.", __FILE__);
#endif
        }

        expected_checksum = read_big_endian_value<uint32_t>(&cdata[4]);
        real_checksum = crc32(0L, (const unsigned char *) &cdata[8], 16);
        if (expected_checksum != real_checksum)
        {
            Logger::error("Bad checksum");
#ifdef WRPG_EXCEPTIONS
            throw TilesetException("%s, Bad checksum", __FILE__);
#endif
        }
        set_version = read_big_endian_value<uint32_t>(&cdata[8]);
        // TODO : version check

        // TODO : See if we need to remove the filename or not as the
        // spritesheet will, as there is an issue on how to load textures.

        // memcpy(sheet_filename, &cdata[12], sizeof(unsigned char) * 8);
        set_sprite_size = read_big_endian_value<uint32_t>(&cdata[20]);
        nb_tiles = read_big_endian_value<uint16_t>(&cdata[22]);
        Logger::log("nb_tiles : %d", nb_tiles);
        chips = new Tilechip[nb_tiles];

        for (int i = 0; i < nb_tiles; i++)
        {
            chips[i].collision_mask = read_big_endian_value<uint8_t>(
                &cdata[WalrusRPG::Tileset::TILESET_HEADER_SIZE +
                       i * WalrusRPG::Tileset::TILESET_CHIP_DATA_SIZE]);
        }
    }
}

Tileset::Tileset(WalrusRPG::Graphics::Texture &tex, uint8_t *collision_masks,
                 uint16_t nb_tiles)
    : anim(), tex(tex), nb_tiles(nb_tiles), chips(nullptr)
{
    chips = new Tilechip[nb_tiles];
    for (int i = 0; i < nb_tiles; i++)
    {
        chips[i].collision_mask = collision_masks[i];
    }
    nb_tile_width = tex.get_dimensions().width / TILE_DIMENSION;
    nb_tile_height = tex.get_dimensions().height / TILE_DIMENSION;
}

Tileset::Tileset(File &tset_data, Texture &tex) : anim(), tex(tex), chips(nullptr)
{
    nb_tile_width = tex.get_dimensions().width / TILE_DIMENSION;
    nb_tile_height = tex.get_dimensions().height / TILE_DIMENSION;
    load_tileset((void *) tset_data.get(), tset_data.file_size, nb_tiles, chips);
}

Tileset::~Tileset()
{
    delete[] this->chips;
}

const Texture &Tileset::get_texture() const
{
    return tex;
}

Rect Tileset::get_tile(uint16_t id) const
{
    return {(id % nb_tile_width) * TILE_DIMENSION, (id / nb_tile_width) * TILE_DIMENSION,
            TILE_DIMENSION, TILE_DIMENSION};
}

uint8_t Tileset::get_collision(uint16_t id) const
{
    if (id >= nb_tiles)
        return 0;
    return chips[id].collision_mask;
}

void Tileset::render_tile(uint16_t id, int x, int y) const
{
    Graphics::put_sprite(tex, x, y, get_tile(anim.get_animation_frame(id)));
}

void Tileset::render_collision_mask(uint16_t id, int x, int y) const
{
    if (id >= nb_tiles)
        return;
    const int half_dim = TILE_DIMENSION / 2;
    uint8_t mask = chips[id].collision_mask;
    if (mask == 0)
        return;
    if (mask & TOP_LEFT_CORNER)
        Graphics::put_rectangle({x, y, half_dim, half_dim}, White);
    if (mask & TOP_RIGHT_CORNER)
        Graphics::put_rectangle({x + half_dim, y, half_dim, half_dim}, White);
    if (mask & BOTTOM_LEFT_CORNER)
        Graphics::put_rectangle({x, y + half_dim, half_dim, half_dim}, White);
    if (mask & BOTTOM_RIGHT_CORNER)
        Graphics::put_rectangle({x + half_dim, y + half_dim, half_dim, half_dim}, White);
}

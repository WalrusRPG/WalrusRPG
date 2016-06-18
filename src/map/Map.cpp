#include "Map.h"
#include "render/Text.h"
#include <zlib.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include "render/TileRenderer.h"
#include "Graphics.h"
#include "Logger.h"
#include "utility/Rect.h"
#include "collision/Collision.h"
#include "utility/misc.h"
#include "piaf/Archive.h"

using WalrusRPG::Entity;
using WalrusRPG::Map;
using WalrusRPG::MapCompression;
using WalrusRPG::MapException;
using namespace WalrusRPG;
using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::Utils;
using namespace WalrusRPG::PIAF;
using WalrusRPG::Graphics::Texture;
using tinystl::vector;

// Graphics::Texture tex_overworld((char *) overworld);
namespace
{
    void load_map(void *data, uint32_t datasize, uint16_t &map_width,
                  uint16_t &map_height, uint16_t *&layer0, uint16_t *&layer1,
                  uint16_t *&layer2)
    {
        char *cdata = (char *) data;
        uint32_t map_version;
        uint32_t expected_checksum, real_checksum;
        uint32_t expected_data_size, real_data_size;
        uint16_t map_n_layers = 0, map_n_events;
        enum MapCompression map_compression =
            MapCompression::RAW; // or whatever Map_Compression will be

        // Basic NPE check
        if (cdata == NULL)
        {
            Logger::error("Null pointer Exception");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Null pointer Exception", __FILE__);
#endif
        }
        // Check for datasize
        if (datasize < 28)
        {
            Logger::error("File too small for header");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: File too small for header", __FILE__);
#endif
        }
        // Magic cookie! Who wants cookies?
        if (strncmp(cdata, "WMap", 4))
        {
            Logger::error("Bad map header");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Bad map header", __FILE__);
#endif
        }

        // Checks for header integrity.
        expected_checksum = read_big_endian_value<uint32_t>(&cdata[4]);
        real_checksum = crc32(0L, (unsigned char *) &cdata[8], 20);
        if (expected_checksum != real_checksum)
        {
            Logger::error("Bad chekcsum");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Bad chekcsum", __FILE__);
#endif
        }
        // Check the map version.
        map_version = read_big_endian_value<uint32_t>(&cdata[8]);
        if (map_version != MAP_VERSION)
        {
            Logger::error("Bad map version");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Bad map version (0x%x != 0x%x)", __FILE__,
                               map_version, MAP_VERSION);
#endif
        }
        // I think you got the drill now, no?
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
        if (map_n_layers < 1 || map_n_layers > 3)
        {
            Logger::error("Wrong map layer number");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Wrong map layer number", __FILE__);
#endif
        }

        // SHould this belong to the meta file?
        map_n_events = read_big_endian_value<uint16_t>(&cdata[22]);
        if (map_n_events != 0)
        {
            Logger::warn("Events not supported yet");
#ifdef WRPG_EXCEPTIONS
            throw MapException("%s: Events not supported yet", __FILE__);
#endif
        }

        map_compression = (MapCompression) read_big_endian_value<uint32_t>(&cdata[24]);

        // We get to *that* part, buckle up.
        layer0 = new uint16_t[map_width * map_height];
        layer1 = new uint16_t[map_width * map_height];
        layer2 = new uint16_t[map_width * map_height];
        switch (map_compression)
        {
            // copy layers from &udata[6] and
            // &udata[6*(map_width*map_height/sizeof(uint32_t))]
            case MapCompression::RAW:
                if (map_n_layers < 1)
                    break;
                // Ground layer
                for (unsigned i = 0, limit = map_width * map_height; i < limit; ++i)
                {
                    layer0[i] = read_big_endian_value<uint16_t>(
                        &cdata[28 + sizeof(uint16_t) * i]);
                }
                if (map_n_layers < 2)
                    break;
                // Middle layer
                for (unsigned i = 0, limit = map_width * map_height; i < limit; ++i)
                {
                    layer1[i] = read_big_endian_value<uint16_t>(
                        &cdata[28 + sizeof(uint16_t) * (map_width * map_height + i)]);
                }
                if (map_n_layers < 3)
                    break;
                // Over layer
                for (unsigned i = 0, limit = map_width * map_height; i < limit; ++i)
                {
                    layer2[i] = read_big_endian_value<uint16_t>(
                        &cdata[28 + sizeof(uint16_t) * (map_width * map_height * 2 + i)]);
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

    bool quadrant_check(Rect &object, uint8_t collision_mask, int x, int y)
    {
        // Half tile stuff happens.
        // Top left corner
        if ((collision_mask & TOP_LEFT_CORNER) &&
            AABBCheck(object,
                      {Tileset::TILE_DIMENSION * x, Tileset::TILE_DIMENSION * y,
                       Tileset::TILE_HALF_DIMENSION, Tileset::TILE_HALF_DIMENSION}))
            return true;
        // Top ~dog~ right corner
        if ((collision_mask & TOP_RIGHT_CORNER) &&
            AABBCheck(object,
                      Rect{Tileset::TILE_DIMENSION * x + Tileset::TILE_HALF_DIMENSION,
                           Tileset::TILE_DIMENSION * y, Tileset::TILE_HALF_DIMENSION,
                           Tileset::TILE_HALF_DIMENSION}))
            return true;
        // Bottom left corner
        if ((collision_mask & BOTTOM_LEFT_CORNER) &&
            AABBCheck(object,
                      {Tileset::TILE_DIMENSION * x,
                       Tileset::TILE_DIMENSION * y + Tileset::TILE_HALF_DIMENSION,
                       Tileset::TILE_HALF_DIMENSION, Tileset::TILE_HALF_DIMENSION}))
            return true;
        // Bottom right
        if ((collision_mask & BOTTOM_RIGHT_CORNER) &&
            AABBCheck(object,
                      {Tileset::TILE_DIMENSION * x + Tileset::TILE_HALF_DIMENSION,
                       Tileset::TILE_DIMENSION * y + Tileset::TILE_HALF_DIMENSION,
                       Tileset::TILE_HALF_DIMENSION, Tileset::TILE_HALF_DIMENSION}))
            return true;
        return false;
    }
}


Map::Map(Archive &data_container, const char *map_filename, const char *tset_filename,
         const char *tex_filename)
    : tmap(data_container.get(tset_filename), data_container.get(tex_filename))
{
    File map_file = data_container.get(map_filename);
    load_map((void *) map_file.get(), map_file.file_size, width, height, layer0, layer1,
             layer2);
}

Map::~Map()
{
}

void Map::update()
{
    // TODO update map's data according to elasped time
    tmap.anim.update();
    for (vector<Entity *>::iterator ptr = entities.begin(); ptr < entities.end(); ++ptr)
    {
        Entity *e = *ptr;
        e->update();

        // Entity deplacment managment
        if (e->moving)
        {
            // Splitting entity movement by axis.
            // Y axis
            float y_sigma = e->vy < 0. ? -1. : 1.;
            float vy = 0;
            // Checking for each pixel if a collision happens.
            // Todo ; initial target collision check so the step-by-step process shouldn't
            // be
            // done all the time
            while (vy != e->vy)
            {
                float add = (std::fabs(vy + y_sigma) > std::fabs(e->vy)) ?
                                (y_sigma * std::fabs(e->vy - vy)) :
                                y_sigma;
                Rect projected{(int) e->x, (int) (e->y + vy + add), e->w, e->h};
                if (object_collision(projected))
                    break;
                if (entity_entity_collision(projected, e))
                    break;
                vy += add;
            }
            e->y += vy;

            // X axis (after updating the y position)
            float x_sigma = e->vx < 0. ? -1. : 1.;
            float vx = 0.;
            // Checking for each pixel if a collision happens.
            // Todo ; initial target collision check so the step-by-step process shouldn't
            // be
            // done all the time
            while (vx != e->vx)
            {
                float add = (std::fabs(vx + x_sigma) > std::fabs(e->vx)) ?
                                (x_sigma * std::fabs(e->vx - vx)) :
                                x_sigma;
                Rect projected{(int) (e->x + vx + add), (int) e->y, e->w, e->h};
                if (object_collision(projected))
                    break;
                if (entity_entity_collision(projected, e))
                    break;
                vx += add;
            }
            e->x += vx;
        }
    }

    // Post update sort for the render part, making the sweep placement faster.
    // This may be the only time a lambda can be useful to me in C++.
    std::sort(entities.begin(), entities.end(), [](Entity *a, Entity *b)
              {
                  return a->y < b->y;
              });
}

// Renders the ground layer.
// If we have three routines, it's only because the inner loop is customised for mutliple
// reasons
// TODO : check if lambda can be useful to factorize the code without losing performances
void Map::render_lower_layer(WalrusRPG::Camera &camera)
{
    if (this->layer0 == nullptr)
        return;
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
    for (signed j = 0; j < delta_y; j++)
    {
        for (signed i = 0; i < delta_x; i++)
        {
            int index, tile_over = 0;
            index = (start_x + i) + (start_y + j) * this->width;
            if (in_range(start_x + i, 0, (signed) width) &&
                in_range(start_y + j, 0, (signed) height))
                tile_over = this->layer0[index];
            tmap.render_tile(tile_over, offset_x + i * t_width, offset_y + j * t_height);
        }
    }
}

// Renders the middle/entity layer. Also draws the entity at the same time so tiles can
// overlap
//  or be overlapped by entities.
// If we have three routines, it's only because the inner loop is customised for mutliple
// reasons
// TODO : check if lambda can be useful to factorize the code without losing performances
void Map::render_entities_layer(WalrusRPG::Camera &camera)
{
    if (this->layer1 == nullptr)
        return;

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

    signed object_y = camera.get_y();
    // WARNING : It supposes the entities are sorted.
    if (!entities.empty())
    {
        object_y = entities[0]->y - camera.get_y();
    }

    size_t index_object = 0;

    // rendering part.
    for (signed j = 0; j < delta_y; j++)
    {
        // Draw each entity until we run out of them or if they are in front of the
        // tiles to be drawn.
        signed y_tile = (start_y + j) * t_height;
        signed y_tile_2 = y_tile + t_height;
        while (index_object < entities.size())
        {
            Entity *e = entities[index_object];
            if (e->y + e->h <= y_tile_2)
            {
                e->render(camera);
                index_object++;
            }
            else
                break;
        }
        for (signed i = 0; i < delta_x; i++)
        {
            int index, tile_over = 0;
            index = (start_x + i) + (start_y + j) * this->width;
            // layer1 : Over-layer
            if (in_range(start_x + i, 0, (signed) width) &&
                in_range(start_y + j, 0, (signed) height))
                tile_over = this->layer1[index];

            if (tile_over != 0)
                tmap.render_tile(tile_over, offset_x + i * t_width,
                                 offset_y + j * t_height);
        }
    }
}

// Render the over layer.
// If we have three routines, it's only because the inner loop is customised for mutliple
// reasons
// TODO : check if lambda can be useful to factorize the code without losing performances
void Map::render_upper_layer(WalrusRPG::Camera &camera)
{
    if (this->layer2 == nullptr)
        return;

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
    for (signed j = 0; j < delta_y; j++)
    {
        for (signed i = 0; i < delta_x; i++)
        {
            int index, tile_over = 0;
            index = (start_x + i) + (start_y + j) * this->width;
            // layer2 : Over-layer
            if (in_range(start_x + i, 0, (signed) width) &&
                in_range(start_y + j, 0, (signed) height))
                tile_over = this->layer2[index];

            if (tile_over != 0)
                tmap.render_tile(tile_over, offset_x + i * t_width,
                                 offset_y + j * t_height);
        }
    }
}

void Map::render(WalrusRPG::Camera &camera)
{
    render_lower_layer(camera);
    render_entities_layer(camera);
    render_upper_layer(camera);
}

void Map::add_entity(Entity *entity)
{
    entities.push_back(entity);
}

// Checks if a tile (ground or middle) does have a collision mask
bool Map::is_tile_solid(int x, int y) const
{
    if (x >= width || y >= height || x < 0 || y < 0)
        return true;
    return (tmap.get_collision(this->layer0[y * width + x]) != 0) ||
           (tmap.get_collision(this->layer1[y * width + x]) != 0);
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

// Checks if a rect collides with an entity.
// The reference only exists to avoid self collisions.
bool Map::entity_entity_collision(Rect a, Entity *ref)
{
    // TODO : QuadTree
    for (auto ptr = entities.begin(); ptr < entities.end(); ptr++)
    {
        Entity *e = *ptr;
        if (e == ref || !e->solid)
            continue;

        Rect b = {(int) e->x, (int) e->y, (unsigned) e->w, (unsigned) e->h};

        if (AABBCheck(a, b))
            return true;
    }
    return false;
}

// Checks if a rect collides with the map.
bool Map::object_collision(Rect object)
{
    int left_tile = object.x / Tileset::TILE_DIMENSION;
    int right_tile = (object.x + object.width - 1) / Tileset::TILE_DIMENSION;
    int top_tile = object.y / Tileset::TILE_DIMENSION;
    int bottom_tile = (object.y + object.height - 1) / Tileset::TILE_DIMENSION;

    if (left_tile < 0)
        left_tile = 0;
    if (right_tile >= width)
        right_tile = width - 1;
    if (top_tile < 0)
        top_tile = 0;
    if (bottom_tile >= height)
        bottom_tile = height - 1;

    for (int i = left_tile; i <= right_tile; i++)
    {
        for (int j = top_tile; j <= bottom_tile; j++)
        {
            // Check if colliding with ground layer
            char t = tmap.get_collision(layer0[j * width + i]);
            if (t && quadrant_check(object, t, i, j))
                return true;

            if (layer1[j * width + i] == 0)
                continue;

            // Check if colliding with middle layer
            t = tmap.get_collision(layer1[j * width + i]);
            if (t && quadrant_check(object, t, i, j))
                return true;
        }
    }
    return false;
}

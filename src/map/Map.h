#ifndef INCLUDE_MAP_H
#define INCLUDE_MAP_H

#include <stdint.h>
#include "TINYSTL/unordered_set.h"
#include "TINYSTL/vector.h"
#include "Entity.h"
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

    constexpr const unsigned MAP_VERSION = 0x00010000;
    // The map containing a virtual gridto manage entity collection.
    // This grid counts in tiles.
    constexpr const unsigned MAP_OBJECT_GRID_TILE_WIDTH = 8;
    constexpr const unsigned MAP_OBJECT_GRID_PIXEL_WIDTH =
        MAP_OBJECT_GRID_TILE_WIDTH * TILE_DIMENSION;

    // TODO : Add tile animations.
    // TODO : Implement compressed data loading routines
    class Map
    {
        friend class States::StateMap;

      public:
        uint16_t *layer0;
        uint16_t *layer1;
        uint16_t *layer2;
        tinystl::unordered_set<Entity *> **entity_container;

      protected:
        uint16_t width;
        uint16_t height;

        // TODO?: add a boolean/getter to know if a second layer exist?

        void get_grid_corners(Utils::Rect r, tinystl::unordered_set<Entity *> *&top_left,
                              tinystl::unordered_set<Entity *> *&top_right,
                              tinystl::unordered_set<Entity *> *&bottom_left,
                              tinystl::unordered_set<Entity *> *&bottom_right);

        void reset_entity_grid();
        void add_entity_to_grid(Entity *e);
        void remove_entity_from_grid(Entity *e);

        void render_lower_layer(Camera &camera);
        void render_entities_layer(Camera &camera);
        void render_upper_layer(Camera &camera);

      public:
        tinystl::vector<Entity *> entities;
        Tileset tmap;

        Map(WalrusRPG::PIAF::Archive &data_container, const char *map_filename,
            const char *tset_filename, const char *tex_filename);
        ~Map();
        void add_entity(Entity *entity);
        void render(Camera &camera);
        void update();
        bool is_tile_solid(int x, int y) const;
        bool is_pixel_solid(int x, int y) const;
        int get_width() const;
        int get_height() const;
        bool entity_entity_collision(WalrusRPG::Utils::Rect a, Entity *ref);
        bool object_collision(WalrusRPG::Utils::Rect object);
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

#include "Map.h"
#include "render/TileRenderer.h"
#include "Graphics.h"
#include "sprites.h"
#include "utility/Rect.h"
#include "utility/misc.h"

using WalrusRPG::Map;
using namespace WalrusRPG;
using namespace WalrusRPG::Utils;
using WalrusRPG::Graphics::Texture;

// Graphics::Texture tex_overworld((char *) overworld);

Map::Map(int width, int height, uint16_t *layer0, uint16_t *layer1, Texture &tex)
    : anim(), tex(tex)
{
    this->renderer = new TileRenderer(tex, 16, 16);
    this->width = width;
    this->height = height;
    this->layer0 = layer0;
    this->layer1 = layer1;
}

Map::~Map()
{
    delete this->renderer;
}

void Map::update(unsigned dt)
{
    // TODO update map's data according to elasped time
    UNUSED(dt);
}

void Map::render(WalrusRPG::Camera &camera, unsigned dt)
{
    anim.update(dt);
    signed t_width = renderer->get_tile_width();
    signed t_height = renderer->get_tile_height();

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
            int index, tile_over;
            index = (start_x + i) + (start_y + j) * this->width;
            if (in_range(start_x + i, 0, (signed) width) &&
                in_range(start_y + j, 0, (signed) height))
                tile_over = anim.get_animation_frame(this->layer0[index]);
            else
                tile_over = 0;
            renderer->render(tile_over,
                             Rect(offset_x + i * t_width, offset_y + j * t_height));

            // layer1 : Over-layer
            if (this->layer1 == nullptr)
                continue;
            if (in_range(start_x + i, 0, (signed) width) &&
                in_range(start_y + j, 0, (signed) height))
                tile_over = anim.get_animation_frame(this->layer1[index]);
            else
                tile_over = 0;
            if (tile_over != 0)
                renderer->render(anim.get_animation_frame(tile_over),
                                 Rect(offset_x + i * t_width, offset_y + j * t_height));
        }
    }
}

bool Map::is_tile_solid(int x, int y) const
{
    if (x >= width || y >= height)
        return true;
    return this->layer0[y * width + x] != 0;
}

bool Map::is_pixel_solid(int x, int y) const
{
    return is_tile_solid(x / renderer->get_tile_width(), y / renderer->get_tile_height());
}

int Map::get_width() const
{
    return this->width;
}

int Map::get_height() const
{
    return this->width;
}

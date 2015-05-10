#include "Map.h"
#include "Camera.h"
#include "Graphics.h"
#include "sprites.h"
#include "misc.h"

#define MAP WalrusRPG::Map

MAP::Map(int width, int height, unsigned *layer0, unsigned *layer1)
    : tset(tiles, 192u, 24u, 24u, 24u), time_render(0)
{
    this->width = width;
    this->height = height;
    this->layer0 = layer0;
    this->layer1 = layer1;
}

MAP::~Map()
{
    // TODO if you allocate dynamically members
}

void MAP::update(unsigned dt)
{
    UNUSED(dt);
    // TODO update map's data according to elasped time
}

void MAP::render(WalrusRPG::Camera &camera, unsigned dt)
{
    time_render += dt;
    // By Eiyeron : I assumed that the camera's position is the top left pixel.
    // Margins moves the rendered map if we go outside of the bounds (specially on the left or on the top).
    signed offset_x = camera.get_x() % 24 * -1;
    signed offset_y = camera.get_y() % 24 * -1;
    signed start_x = camera.get_x() / 24;
    signed start_y = camera.get_y() / 24;
    signed end_x = start_x + 15;
    signed end_y = start_y + 16;

    // Bound-checking code. To avoid reading outside of the map.
    // The offset edition allows the map to be correctly moved to the right to make up for the lack of renderable tiles.
    if (start_x < 0)
    {
        offset_x -= start_x * 24;
        start_x = 0;
    }

    if (start_y < 0)
    {
        offset_y -= start_y * 24;
        start_y = 0;
    }
    // warning fix. Even if end_x/y is negative, it should be higher than width/height.
    if ((unsigned) end_x > this->width)
        end_x = this->width;
    if ((unsigned) end_y > this->height)
        end_y = this->height;

    // pre-calculating variables to speed up loop condition check
    signed delta_x = end_x - start_x;
    signed delta_y = end_y - start_y;

    // Creating a region clip. Why does it smell like SDL?
    /*WalrusRPG::Graphics::Rect_t sprite;
    sprite.y = 0;
    sprite.w = 24;
    sprite.h = 24;*/

    // rendering part.
    for (signed j = 0; j < delta_y; j++)
    {
        for (signed i = 0; i < delta_x; i++)
        {
            unsigned index = (start_x + i) + (start_y + j) * this->width;
            tset.render_tile(this->layer0[index], offset_x + i * 24, offset_y + j * 24, time_render);

            unsigned tile_over = this->layer1[index];
            // layer1 : Over-layer
            if (tile_over != 0)
            {
                //sprite.x = tile_over * 24;
                tset.render_tile(tile_over, offset_x + i * 24, offset_y + j * 24, time_render);
            }
        }
    }
}

bool MAP::is_tile_solid(unsigned x, unsigned y) const
{
    if (x >= width || y >= height)
        return true;
    return this->layer0[y * width + x] != 0;
}

bool MAP::is_pixel_solid(unsigned x, unsigned y) const
{
    return is_tile_solid(x / 24, y / 24);
}

unsigned MAP::get_width() const
{
    return this->width;
}

unsigned MAP::get_height() const
{
    return this->width;
}

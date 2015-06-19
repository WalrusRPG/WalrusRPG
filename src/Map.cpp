#include "Map.h"
#include "Camera.h"
#include "Graphics.h"
#include "sprites.h"
#include "Rect.h"
#include "TileRenderer.h"
#include "misc.h"

#define MAP WalrusRPG::Map
#define RECT WalrusRPG::Utils::Rect
#define TILERENDERER WalrusRPG::TileRenderer

MAP::Map(int width, int height, unsigned *layer0, unsigned *layer1)
    : anim(), time_render(0)
{
    this->renderer = new TileRenderer(overworld, 16, 16);
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
    signed t_width = renderer->get_tile_width();
    signed t_height = renderer->get_tile_height();
    // By Eiyeron : I assumed that the camera's position is the top left pixel.
    // Margins moves the rendered map if we go outside of the bounds (specially on the left or on the top).
    signed offset_x = camera.get_x() % t_width * -1;
    signed offset_y = camera.get_y() % t_height * -1;
    signed start_x = camera.get_x() / t_width;
    signed start_y = camera.get_y() / t_height;
    signed end_x = start_x + 320 / t_width + 2; // Why 2? I don't freaking know.
    signed end_y = start_y + 240 / t_height + 1;

    // Bound-checking code. To avoid reading outside of the map.
    // The offset edition allows the map to be correctly moved to the right to make up for the lack of renderable tiles.
    if (start_x < 0)
    {
        offset_x -= start_x * t_width;
        start_x = 0;
    }

    if (start_y < 0)
    {
        offset_y -= start_y * t_height;
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
            unsigned tile_over = this->layer0[index];
            if (tile_over != 0)
                renderer->render(this->layer0[index], RECT(offset_x + i * t_width, offset_y + j * t_height));
            // layer1 : Over-layer
            if (this->layer1 == NULL)
                continue;
            tile_over = this->layer1[index];
            if (tile_over != 0)
                renderer->render(tile_over, RECT(offset_x + i * t_width, offset_y + j * t_height));
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
    return is_tile_solid(x / renderer->get_tile_width(), y / renderer->get_tile_height());
}

unsigned MAP::get_width() const
{
    return this->width;
}

unsigned MAP::get_height() const
{
    return this->width;
}

#include "StateCollision.h"
#include "Graphics.h"
#include "input/Input.h"
#include "render/Text.h"

using WalrusRPG::Box;
using WalrusRPG::States::StateCollision;
using WalrusRPG::Utils::Rect;
using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::Input;

namespace AABB
{
		// returns true if the boxes are colliding (velocities are not used)
	bool AABBCheck(Box b1, Box b2)
	{
	    return !(b1.x + b1.w <= b2.x || b1.x >= b2.x + b2.w || b1.y + b1.h <= b2.y || b1.y >= b2.y + b2.h);
	}

}

StateCollision::StateCollision()
:p(40, 40, 8, 8, 0, 0), target(p), map{0}, cam(0,0)
{
	srand(4);
	for (int i = 0; i < 10; ++i)
	{
		map[rand()%20][rand()%20] = 1;
		map[rand()%20][rand()%20] = 2;
		map[rand()%20][rand()%20] = 3;
		map[rand()%20][rand()%20] = 4;
		map[rand()%20][rand()%20] = 5;
	}
}

StateCollision::~StateCollision()
{

}

namespace
{
	bool tilemap_collision(char map[20][20], const Box&& object)
	{
		int left_tile = object.x / 16;
		int right_tile = (object.x+object.w-1) / 16;
		int top_tile = object.y / 16;
		int bottom_tile = (object.y + object.h-1) / 16;

		if(left_tile < 0) left_tile = 0;
		if(right_tile > 19) right_tile = 19;
		if(top_tile < 0) top_tile = 0;
		if(bottom_tile > 19) bottom_tile = 19;

		for(int i=left_tile; i<=right_tile; i++)
		{
		    for(int j=top_tile; j<=bottom_tile; j++)
		    {
		        char t = map[j][i];
		        if(t)
		        {
		        	if(t == 1 && AABB::AABBCheck(object, Box(16.*i, 16.*j, 16., 16., 0, 0)))
		            	return true;
		        	if(t == 2 && AABB::AABBCheck(object, Box(16.*i, 16.*j, 16., 8., 0, 0)))
		        		return true;
		        	if(t == 3 && AABB::AABBCheck(object, Box(16.*i, 16.*j+8, 16., 8., 0, 0)))
		        		return true;
		        	if(t == 4 && AABB::AABBCheck(object, Box(16.*i, 16.*j, 8., 16., 0, 0)))
		        		return true;
		        	if(t == 5 && AABB::AABBCheck(object, Box(16.*i+8, 16.*j, 8., 16., 0, 0)))
		        		return true;
		        }
		    }
		}
		return false;
	}
}

void StateCollision::update(unsigned dt)
{
	collided_top = false;
	collided_left = false;
	collided_bottom = false;
	collided_right = false;
	p.vx = 0;
	p.vy = 0;
	if(key_down(Key::K_LEFT))
		p.vx = -2. * dt;
	if(key_down(Key::K_RIGHT))
		p.vx = 2. * dt;
	if(key_down(Key::K_UP))
		p.vy = -2. * dt;
	if(key_down(Key::K_DOWN))
		p.vy = 2. * dt;


	float x_sigma = p.vx < 0. ? -1. : 1.;
	float vx = 0.;
	while(vx != p.vx)
	{
		float add = (std::fabs(vx + x_sigma) > std::fabs(p.vx)) ? (x_sigma * std::fabs(p.vx - vx)) : x_sigma;
		if(tilemap_collision(map, Box(p.x + vx + add, p.y, p.w, p.h, 0, 0)))
			break;
		vx += add;
	}
	p.x += vx;

	float y_sigma = p.vy < 0. ? -1. : 1.;
	float vy = 0;
	while(vy != p.vy)
	{
		float add = (std::fabs(vy + y_sigma) > std::fabs(p.vy)) ? (y_sigma * std::fabs(p.vy - vy)) : y_sigma;
		if(tilemap_collision(map, Box(p.x, p.y + vy + add, p.w, p.h, 0, 0)))
			break;
		vy += add;
	}
	p.y += vy;

	cam.set_center_x(p.x + p.w/2);
	cam.set_center_y(p.y + p.h/2);
}

void StateCollision::render(unsigned dt)
{
	fill(Black);
	Pixel a(Red);

	put_rectangle({static_cast<signed>(p.x)-cam.get_x(), static_cast<signed>(p.y)-cam.get_y(), static_cast<unsigned>(p.w), static_cast<unsigned>(p.h)}, a);
	for (int y = 0; y < 20; ++y)
	{
		for (int x = 0; x < 20; ++x)
		{
			if(map[y][x])
			{
				switch(map[y][x])
				{
					case 1:
					put_rectangle({x*16-cam.get_x(), y*16-cam.get_y(), 16, 16}, LightGray);
					break;
					case 2:
					put_rectangle({x*16-cam.get_x(), y*16-cam.get_y(), 16, 8}, Gray);
					break;
					case 3:
					put_rectangle({x*16-cam.get_x(), y*16-cam.get_y()+8, 16, 8}, DarkGray);
					break;
					case 4:
					put_rectangle({x*16-cam.get_x(), y*16-cam.get_y(), 8, 16}, Cyan);
					break;
					case 5:
					put_rectangle({x*16-cam.get_x()+8, y*16-cam.get_y(), 8, 16}, Blue);
					break;
				}

			}
		}
	}

	Text::print_format(0, 8, "x = %f", p.x);
	Text::print_format(0, 16, "y = %f", p.y);
	Text::print_format(0, 24, "vx = %f", p.vx);
	Text::print_format(0, 32, "vy = %f", p.vy);

}
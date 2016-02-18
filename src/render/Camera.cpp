#include "Camera.h"
#include "utility/misc.h"
#include "input/Input.h"

using WalrusRPG::Camera;
using WalrusRPG::CameraCenterType;
using namespace WalrusRPG;
using WalrusRPG::Input::Key;

Camera::Camera(signed x, signed y, CameraCenterType center_type)
: x(x), y(y), render_area_width(320), render_area_height(240), center_type(center_type)
{
    set_x(x);
    set_y(y);
}

Camera::~Camera()
{
    // TODO if you allocate dynamically members
}

void Camera::update(unsigned dt)
{
    UNUSED(dt);
    // TODO update map's data according to elasped time
    /*
                // Need to think aagain on how to go to a target point and/or we need to
       align the corner OR the center to this point.
                position += velocity * dt;
                velocity += acceleration * dt;
         */

    if (Input::key_down(Key::K_DOWN))
        y++;
    if (Input::key_down(Key::K_UP))
        y--;
    if (Input::key_down(Key::K_RIGHT))
        x++;
    if (Input::key_down(Key::K_LEFT))
        x--;
}

void Camera::set_x(signed x)
{
    this->x = x;
    if(center_type == CENTER)
        this->x -= render_area_width/2;
}

signed Camera::get_x() const
{
    return center_type == CENTER? this->x + render_area_width/2 : this->x;
}

void Camera::set_y(signed y)
{
    this->y = y;
    if(this->center_type == CENTER)
        this->y -= render_area_height/2;
}

signed Camera::get_y() const
{
    return center_type == CENTER? this->y + render_area_height/2 : this->y;
}

bool Camera::is_visible(const WalrusRPG::Utils::Rect &object) const
{
    if ((in_range(object.x, x, x + (signed) render_area_width) ||
         in_range(object.x + (signed) object.width - 1, x,
                  x + (signed) render_area_width)) &&
        (in_range(object.y, y, y + (signed) render_area_height) ||
         in_range(object.y + (signed) object.height - 1, y,
                  y + (signed) render_area_height)))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Camera::set_center_type(CameraCenterType center_type)
{
    this->center_type = center_type;
}

CameraCenterType Camera::get_center_type()
{
    return this->center_type;
}

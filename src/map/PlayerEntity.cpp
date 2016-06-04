#include "PlayerEntity.h"
#include "Map.h"
#include "input/Input.h"
#include "utility/Rect.h"

using WalrusRPG::PlayerEntity;
using WalrusRPG::Map;
using WalrusRPG::Camera;
using WalrusRPG::Utils::Rect;
using namespace WalrusRPG::Input;

PlayerEntity::PlayerEntity(float x, float y, unsigned w, unsigned h,
                           WalrusRPG::Renderer *tset, unsigned sprite_id)
    : Entity(x, y, w, h, tset, sprite_id)
{
    moving = true;
}

PlayerEntity::~PlayerEntity()
{
}


void PlayerEntity::update(unsigned dt)
{
    vx = 0;
    vy = 0;
    if (key_down(Key::K_LEFT))
        vx = -1. * dt;
    if (key_down(Key::K_RIGHT))
        vx = 1. * dt;
    if (key_down(Key::K_UP))
        vy = -1. * dt;
    if (key_down(Key::K_DOWN))
        vy = 1. * dt;
}

void PlayerEntity::render(Camera &camera, unsigned dt) const
{
    if (tset == nullptr)
        return;
    if (camera.is_visible({(int) x, (int) y, w, h}))
    {
        tset->render(sprite_id, Rect(x - camera.get_x() - 3, y - camera.get_y() - 12));
        //(*tset).render_tile(sprite_id, coords.x - camera.get_x(), coords.y -
        // camera.get_y(), dt);
    }
}

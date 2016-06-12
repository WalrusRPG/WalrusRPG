#include "PlayerEntity.h"
#include "StateMap.h"
#include "Map.h"
#include "input/Input.h"
#include "utility/Rect.h"

using WalrusRPG::Animation;
using WalrusRPG::PlayerEntity;
using WalrusRPG::Map;
using WalrusRPG::Camera;
using WalrusRPG::Utils::Rect;
using namespace WalrusRPG::Input;

PlayerEntity::PlayerEntity(States::StateMap &container, float x, float y, unsigned w,
                           unsigned h, WalrusRPG::Renderer *tset, unsigned sprite_id)
    : Entity(container, x, y, w, h, tset, sprite_id), direction(0)
{
    moving = true;


    tinystl::vector<WalrusRPG::Frame> up{1, {176, 1}};
    tinystl::vector<WalrusRPG::Frame> down{1, {128, 1}};
    tinystl::vector<WalrusRPG::Frame> left{1, {144, 1}};
    tinystl::vector<WalrusRPG::Frame> right{1, {160, 1}};


    tinystl::vector<WalrusRPG::Frame> walk_up;
    walk_up.push_back({176, 10});
    walk_up.push_back({177, 10});
    walk_up.push_back({176, 10});
    walk_up.push_back({178, 10});

    tinystl::vector<WalrusRPG::Frame> walk_down;
    walk_down.push_back({128, 10});
    walk_down.push_back({129, 10});
    walk_down.push_back({128, 10});
    walk_down.push_back({130, 10});

    tinystl::vector<WalrusRPG::Frame> walk_left;
    walk_left.push_back({144, 10});
    walk_left.push_back({145, 10});
    walk_left.push_back({144, 10});
    walk_left.push_back({146, 10});

    tinystl::vector<WalrusRPG::Frame> walk_right;
    walk_right.push_back({160, 10});
    walk_right.push_back({161, 10});
    walk_right.push_back({160, 10});
    walk_right.push_back({162, 10});

    animator.add_animation(0, {up, false});
    animator.add_animation(1, {down, false});
    animator.add_animation(2, {left, false});
    animator.add_animation(3, {right, false});
    animator.add_animation(4, {walk_up, true});
    animator.add_animation(5, {walk_down, true});
    animator.add_animation(6, {walk_left, true});
    animator.add_animation(7, {walk_right, true});
}

PlayerEntity::~PlayerEntity()
{
}


void PlayerEntity::update(unsigned dt)
{
    vx = 0;
    vy = 0;
    if (key_down(Key::K_LEFT))
    {
        vx = -1. * dt;
        direction = 2;
    }
    if (key_down(Key::K_RIGHT))
    {
        vx = 1. * dt;
        direction = 3;
    }
    if (key_down(Key::K_UP))
    {
        vy = -1. * dt;
        direction = 0;
    }
    if (key_down(Key::K_DOWN))
    {
        vy = 1. * dt;
        direction = 1;
    }
    animator.update(dt);
}

void PlayerEntity::render(Camera &camera, unsigned dt) const
{
    if (tset == nullptr)
        return;
    if (camera.is_visible({(int) x, (int) y, w, h}))
    {
        int offset = 0;
        if (vx != 0 || vy != 0)
            offset = 4;
        tset->render(animator.get_animation_frame(direction + offset),
                     Rect(x - camera.get_x() - 3, y - camera.get_y() - 12));
        //(*tset).render_tile(sprite_id, coords.x - camera.get_x(), coords.y -
        // camera.get_y(), dt);
    }
}

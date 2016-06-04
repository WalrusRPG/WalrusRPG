#include "PlayerEntity.h"
#include "Map.h"
#include "input/Input.h"

using WalrusRPG::PlayerEntity;
using WalrusRPG::Map;
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
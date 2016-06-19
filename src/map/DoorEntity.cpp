#include "DoorEntity.h"
#include "StateMap.h"
#include "utility/misc.h"

using WalrusRPG::DoorEntity;
using WalrusRPG::Camera;

DoorEntity::DoorEntity(States::StateMap &container, float x, float y, unsigned w,
                       unsigned h, WalrusRPG::Renderer *tset, unsigned sprite_closed_id,
                       unsigned sprite_opened_id)
    : Entity(container, x, y, w, h, tset, sprite_closed_id),
      sprite_closed_id(sprite_closed_id), sprite_opened_id(sprite_opened_id),
      opened(false)
{
}

DoorEntity::~DoorEntity()
{
}

void DoorEntity::update()
{
}

void DoorEntity::render(Camera &camera) const
{
    Entity::render(camera);
}

void DoorEntity::interact_with(Entity &origin, InteractionType type)
{
    UNUSED(origin);
    UNUSED(type);
    if (!opened)
    {
        opened = true;
        sprite_id = sprite_opened_id;
        solid = false;
    }
}
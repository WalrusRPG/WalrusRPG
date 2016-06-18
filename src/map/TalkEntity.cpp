#include "TalkEntity.h"
#include "StateMap.h"
#include "utility/misc.h"

using WalrusRPG::TalkEntity;
using WalrusRPG::Camera;

TalkEntity::TalkEntity(States::StateMap &container, float x, float y, unsigned w,
                       unsigned h, WalrusRPG::Renderer *tset, unsigned sprite_id,
                       const char *msg)
    : Entity(container, x, y, w, h, tset, sprite_id), msg(msg)
{
}

TalkEntity::~TalkEntity()
{
}

void TalkEntity::update()
{
}

void TalkEntity::render(Camera &camera) const
{
    Entity::render(camera);
}

void TalkEntity::interact_with(Entity &origin, InteractionType type)
{
    UNUSED(origin);
    UNUSED(type);
    container.box.set_text(msg);
    container.started = true;
    container.p.controllable = false;
}
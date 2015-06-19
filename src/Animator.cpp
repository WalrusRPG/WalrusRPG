#include "Animator.h"
#include "Graphics.h"
#include "Rect.h"

#define ANIMATOR WalrusRPG::Animator
#define FRAME WalrusRPG::Frame
#define UTILS WalrusRPG::Utils

namespace
{
    unsigned find_frame(const tinystl::vector<WalrusRPG::Frame> &anim, signed frame_time)
    {
        unsigned index = 0;
        do
        {
            frame_time -= anim[index].duration;
            index = (index + 1) % anim.size();
        } while (frame_time > 0);
        return index;
    }
}

ANIMATOR::Animator()
{
}

void ANIMATOR::add_animation(int index, Animation anim)
{
    animations[index] = anim;
}

unsigned ANIMATOR::get_animation_frame(unsigned id)
{
    if (animations[id].stripe.empty())
        return id;
    return find_frame(animations[id].stripe, elapsed_time);
}

void ANIMATOR::update(unsigned dt)
{
    elapsed_time += dt;
}

#include "Timing.h"
#include <SFML/System/Clock.hpp>
#include <cstdio>

#define TIMING WalrusRPG::Timing

sf::Clock clock;

void TIMING::init()
{
}

void TIMING::deinit()
{
}

unsigned TIMING::gettime()
{
    return clock.getElapsedTime().asMicroseconds();
}

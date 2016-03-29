#include "Timing.h"
#include "Logger.h"
#include <SFML/System/Clock.hpp>
#include <cstdio>

using namespace WalrusRPG; /*Timing*/

sf::Clock clock;

void Timing::init()
{
    Logger::log("Timing init");
}

void Timing::deinit()
{
    Logger::log("Timing deinit");
}

unsigned Timing::gettime()
{
    return clock.getElapsedTime().asMicroseconds();
}

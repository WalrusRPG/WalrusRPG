#include "Status.h"
#include "Logger.h"
#include "Quirks.h"
#include "sfwindow.h"
#include <SFML/Window/Event.hpp>
#include "platform.h"

using namespace WalrusRPG;

namespace
{
    static bool askedToQuit;
}

void Status::init()
{
    Logger::log("Status init");
    askedToQuit = false;
}

void Status::deinit()
{
    Logger::log("Status deinit");
}

void Status::update()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            askedToQuit = true;
    }
    if (Quirks::get_key(sf::Keyboard::Escape))
        askedToQuit = true;
}

bool Status::mustQuit()
{
    return askedToQuit;
}

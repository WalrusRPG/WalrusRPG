#include <cstdlib>
#include "StateMachine.h"
#include "Timing.h"
#include "Logger.h"
#include "render/Text.h"
#include "Graphics.h"
#include "Quirks.h"
#include "Status.h"
#include "map/Map.h"
#include "map/StateMap.h"
#include "piaf/Archive.h"
#include "utility/misc.h"

using namespace WalrusRPG;
using WalrusRPG::PIAF::Archive;
using WalrusRPG::PIAF::File;
using WalrusRPG::Graphics::Texture;
using namespace WalrusRPG::Graphics;


int main(int argc, char *argv[])
{
    UNUSED(argc);
    Graphics::init();
    Logger::init();
    Logger::log("WalrusRPG Init");
    Status::init();
    Timing::init();
    Quirks::init(argv[0]);

    Text::init();
    Archive arc("data/wip_data.wrf");
    Map map(arc, "map.wrm", "set.wts", "castle.png");
    tinystl::vector<Frame> stripe80;
    tinystl::vector<Frame> stripe126;

    tinystl::vector<Frame> stripe205;
    tinystl::vector<Frame> stripe221;
    tinystl::vector<Frame> stripe237;
    tinystl::vector<Frame> stripe253;
    for (int i = 0; i < 15; i++)
    {
        static unsigned int frames[]{80, 81, 82, 96, 97, 98, 112, 113, 114};
        stripe80.push_back({frames[rand() % 9], 10u + (rand() % 20u)});
    }
    stripe126.push_back({126, 15});
    stripe126.push_back({127, 15});

    stripe205.push_back({205, 10});
    stripe205.push_back({206, 10});
    stripe205.push_back({207, 10});

    stripe221.push_back({221, 10});
    stripe221.push_back({222, 10});
    stripe221.push_back({223, 10});

    stripe237.push_back({237, 10});
    stripe237.push_back({238, 10});
    stripe237.push_back({239, 10});

    stripe253.push_back({253, 10});
    stripe253.push_back({254, 10});
    stripe253.push_back({255, 10});


    map.tmap.anim.add_animation(80, {stripe80, true, 0});
    map.tmap.anim.add_animation(126, {stripe126, true, 0});
    map.tmap.anim.add_animation(205, {stripe205, true, 0});
    map.tmap.anim.add_animation(221, {stripe221, true, 0});
    map.tmap.anim.add_animation(237, {stripe237, true, 0});
    map.tmap.anim.add_animation(253, {stripe253, true, 0});

    StateMachine::init();
    StateMachine::push(new States::StateMap(0, 0, map));
    StateMachine::run();

    Logger::log("WalrusRPG Deinit");
    StateMachine::deinit();
    Text::deinit();

    Quirks::deinit();
    Timing::deinit();
    Status::deinit();
    Logger::log("WalrusRPG Exit");
    Graphics::deinit();

    return 0;
}

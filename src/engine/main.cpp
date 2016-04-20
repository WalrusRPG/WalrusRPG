#include "StateMachine.h"
#include "Timing.h"
#include "Logger.h"
#include "render/Text.h"
#include "Graphics.h"
#include "Quirks.h"
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
    Logger::log("WalrusRPG Init");
    Graphics::init();
    Timing::init();
    Quirks::init(argv[0]);
    Text::init();

    Archive arc("data/wip_data.wrf");
    Texture tex(arc.get("castle.png"));
    /*
    WalrusRPG::PIAF::File f1 = arc.get("l1.bin");
    WalrusRPG::PIAF::File f2 = arc.get("l2.bin");

    const uint8_t *l1 = f1.get();
    const uint8_t *l2 = f2.get();

    // TODO better map reading.
    uint16_t *dungeonTest = new uint16_t[f1.file_size / 2 + 1];
    uint16_t *dungeonTest2 = new uint16_t[f1.file_size / 2 + 1];

    for (unsigned i = 0; i < f1.file_size / 2; i++)
    {
        dungeonTest[i] = read_big_endian_value<uint16_t>(&l1[i * 2]);
        dungeonTest2[i] = read_big_endian_value<uint16_t>(&l2[i * 2]);
    }
    */
  File f = arc.get("map.wrm");
    Map map(f, tex);
    tinystl::vector<Frame> stripe21;
    tinystl::vector<Frame> stripe22;
    stripe21.push_back({21, 23});
    stripe21.push_back({22, 31});
    stripe22.push_back({22, 37});
    stripe22.push_back({21, 41});
    map.tmap.anim.add_animation(21, {stripe21, true, 0});
    map.tmap.anim.add_animation(22, {stripe22, true, 0});

    StateMachine::init();
    StateMachine::push(new States::StateMap(0, 0, map));
    StateMachine::run();

    Logger::log("WalrusRPG Deinit");
    StateMachine::deinit();
    Quirks::deinit();
    Timing::deinit();
    Graphics::deinit();
    // delete[] dungeonTest;
    // delete[] dungeonTest2;
    Logger::log("WalrusRPG Exit");

    return 0;
}

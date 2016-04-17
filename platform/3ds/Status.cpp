#include "Status.h"
#include "Logger.h"
#include "Quirks.h"
#include "platform.h"
#include <3ds/services/apt.h>

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
  if(!aptMainLoop())
    askedToQuit = true;
  if(Quirks::get_key(KEY_ZL))
    askedToQuit = true;
}

bool Status::mustQuit()
{
    return askedToQuit;
}

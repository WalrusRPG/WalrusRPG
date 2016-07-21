#include "Status.h"
#include "Logger.h"
#include "Quirks.h"

using namespace WalrusRPG; /*Status*/

namespace
{
    static bool askedForQuit;
}

void Status::init()
{
    Logger::log("Logger init");
    askedForQuit = false;
}

void Status::deinit()
{
    Logger::log("Logger deinit");
}

void Status::update()
{
}

bool Status::mustQuit()
{
    return WalrusRPG::Quirks::get_key(KEY_NSPIRE_ESC);
}

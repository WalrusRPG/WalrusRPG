#include "Timing.h"
#include "Logger.h"
#include <cstdio>

using namespace WalrusRPG; /*Timing*/

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
  return 1;
}

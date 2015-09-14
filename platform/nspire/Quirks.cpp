#include "Quirks.h"
#include "Interrupts.h"

using namespace WalrusRPG;
using namespace Nspire;

void Quirks::init()
{
    Interrupts::init();
}

void Quirks::deinit()
{
    Interrupts::off();
}

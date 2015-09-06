#include "Quirks.h"
#include "Interrupts.h"

#define QUIRKS WalrusRPG::Quirks
using namespace Nspire;

void QUIRKS::init()
{
    Interrupts::init();
}

void QUIRKS::deinit()
{
    Interrupts::off();
}

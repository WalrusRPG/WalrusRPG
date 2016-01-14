#include "Quirks.h"
#include "Interrupts.h"
#include "platform.h"

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

bool Quirks::get_key(keycode_t key)
{
    return isKeyPressed(key);
}

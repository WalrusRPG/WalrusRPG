#include "Input.h"
#include <libndls.h>

#define INPUT WalrusRPG::Input

bool INPUT::key_a()
{
    return isKeyPressed(KEY_NSPIRE_ENTER);
}

bool INPUT::key_b()
{
    return isKeyPressed(KEY_NSPIRE_ESC);
}

bool INPUT::key_up()
{
    return isKeyPressed(KEY_NSPIRE_8);
}

bool INPUT::key_down()
{
    return isKeyPressed(KEY_NSPIRE_5);
}

bool INPUT::key_left()
{
    return isKeyPressed(KEY_NSPIRE_4);
}

bool INPUT::key_right()
{
    return isKeyPressed(KEY_NSPIRE_6);
}

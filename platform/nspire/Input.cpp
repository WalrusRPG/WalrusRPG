#include "Input.h"
#include <libndls.h>

#define INPUT WalrusRPG::Input

bool INPUT::key_a()
{
    return isKeyPressed(KEY_NSPIRE_CTRL);
}

bool INPUT::key_b()
{
    return isKeyPressed(KEY_NSPIRE_SHIFT);
}

bool INPUT::key_l()
{
    return isKeyPressed(KEY_NSPIRE_TAB);
}

bool INPUT::key_r()
{
    return isKeyPressed(KEY_NSPIRE_MENU);
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

bool INPUT::key_start()
{
    return isKeyPressed(KEY_NSPIRE_HOME);
}

bool INPUT::key_select()
{
    return isKeyPressed(KEY_NSPIRE_ESC);
}

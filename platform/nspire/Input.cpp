#include "Input.h"
#include <libndls.h>

#define INPUT WalrusRPG::Input

using WalrusRPG::Input::Key;
using WalrusRPG::Input::KeyState;

struct InputMap
{
    Key key;
    t_key key_code;
};

static KeyState key_states[Key::K_SIZE] = {KeyState::KS_RELEASED};
static InputMap key_map[] = {
    {Key::K_A, KEY_NSPIRE_CTRL},     {Key::K_B, KEY_NSPIRE_SHIFT},
    {Key::K_L, KEY_NSPIRE_TAB},      {Key::K_R, KEY_NSPIRE_MENU},

    {Key::K_UP, KEY_NSPIRE_8},       {Key::K_DOWN, KEY_NSPIRE_5},
    {Key::K_LEFT, KEY_NSPIRE_4},     {Key::K_RIGHT, KEY_NSPIRE_6},

    {Key::K_START, KEY_NSPIRE_HOME}, {Key::K_SELECT, KEY_NSPIRE_ESC},
};

void INPUT::key_poll()
{
    for (unsigned i = 0; i < K_SIZE; i++)
    {
        bool current_key_state = isKeyPressed(key_map[i].key_code);
        KeyState previous_key_state = key_states[i];

        KeyState resulting_key_state = KS_RELEASED;
        if (current_key_state)
        {
            if (previous_key_state == KS_RELEASED ||
                previous_key_state == KS_JUST_RELEASED)
                resulting_key_state = KS_JUST_PRESSED;
            else if (previous_key_state == KS_JUST_PRESSED ||
                     previous_key_state == KS_PRESSED)
                resulting_key_state = KS_PRESSED;
        }
        else
        {
            if (previous_key_state == KS_PRESSED || previous_key_state == KS_JUST_PRESSED)
                resulting_key_state = KS_JUST_RELEASED;
            else if (previous_key_state == KS_JUST_RELEASED ||
                     previous_key_state == KS_RELEASED)
                resulting_key_state = KS_RELEASED;
        }
        key_states[i] = resulting_key_state;
    }
}

bool INPUT::key_pressed(Key key)
{
    return key_states[key] == KS_JUST_PRESSED;
}
bool INPUT::key_released(Key key)
{
    return key_states[key] == KS_JUST_RELEASED;
}
bool INPUT::key_down(Key key)
{
    return key_states[key] == KS_JUST_PRESSED || key_states[key] == KS_PRESSED;
}
bool INPUT::key_up(Key key)
{
    return key_states[key] == KS_JUST_RELEASED || key_states[key] == KS_RELEASED;
}

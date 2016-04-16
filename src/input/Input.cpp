#include "input/Input.h"
#include "Quirks.h"
#include "platform.h"

using namespace WalrusRPG; /*::Input*/
using WalrusRPG::Input::Key;
using WalrusRPG::Input::KeyState;

struct InputMap
{
    Key key;
    keycode_t key_code;
};

struct KeyBuffer
{
    bool current;
    bool previous;
};
struct KeyBuffer key_states[Key::K_SIZE] = {{false, false}};

// TODO: make these software-mappable
#ifdef TARGET_SFML
using sf::Keyboard;
InputMap key_map[] = {
    {Key::K_A, Keyboard::W},          {Key::K_B, Keyboard::X},
    {Key::K_L, Keyboard::Q},          {Key::K_R, Keyboard::S},

    {Key::K_UP, Keyboard::Up},        {Key::K_DOWN, Keyboard::Down},
    {Key::K_LEFT, Keyboard::Left},    {Key::K_RIGHT, Keyboard::Right},

    {Key::K_START, Keyboard::Return}, {Key::K_SELECT, Keyboard::BackSpace},
};
#endif
#ifdef TARGET_NSPIRE
static InputMap key_map[] = {
    {Key::K_A, KEY_NSPIRE_CTRL},    {Key::K_B, KEY_NSPIRE_SHIFT},
    {Key::K_L, KEY_NSPIRE_TAB},     {Key::K_R, KEY_NSPIRE_MENU},

    {Key::K_UP, KEY_NSPIRE_8},      {Key::K_DOWN, KEY_NSPIRE_5},
    {Key::K_LEFT, KEY_NSPIRE_4},    {Key::K_RIGHT, KEY_NSPIRE_6},

    {Key::K_START, KEY_NSPIRE_DOC}, {Key::K_SELECT, KEY_NSPIRE_ESC},
};
#endif
#ifdef TARGET_3DS
static InputMap key_map[] = {
    {Key::K_A, KEY_A},    {Key::K_B, KEY_B},
    {Key::K_L, KEY_L},     {Key::K_R, KEY_R},

    {Key::K_UP, KEY_UP},      {Key::K_DOWN, KEY_DOWN},
    {Key::K_LEFT, KEY_LEFT},    {Key::K_RIGHT, KEY_RIGHT},

    {Key::K_START, KEY_START}, {Key::K_SELECT, KEY_SELECT},
};
u32 kDown = 0;
u32 kHeld = 0;
u32 kUp = 0;
#endif

KeyState Input::key_get_state(Key key)
{
    // "Just" pressed/released before held/inactive to let these events through
    if (key_pressed(key))
        return KS_JUST_PRESSED;
    if (key_down(key))
        return KS_PRESSED;
    if (key_released(key))
        return KS_JUST_RELEASED;
    if (key_up(key))
        return KS_RELEASED;

    // Default case to mute compiler warnings, shouldn't even reach here in practice
    return KS_RELEASED;
}

void Input::key_poll()
{
  #ifdef TARGET_3DS
    hidScanInput();
    kDown = hidKeysDown();
    kHeld = hidKeysHeld();
    kUp = hidKeysUp();
  #else
  for (unsigned i = 0; i < K_SIZE; i++)
  {
      key_states[i].previous = key_states[i].current;
      key_states[i].current = WalrusRPG::Quirks::get_key(key_map[i].key_code);
  }
  #endif
}

bool Input::key_pressed(Key key)
{
  #ifdef TARGET_3DS
    return kDown & key_map[key].key_code;
  #else
    return !key_states[key].previous && key_states[key].current;
  #endif
}

bool Input::key_released(Key key)
{
  #ifdef TARGET_3DS
    return kUp & key_map[key].key_code;
  #else
    return key_states[key].previous && !key_states[key].current;
  #endif
}
bool Input::key_down(Key key)
{
  #ifdef TARGET_3DS
    return kHeld & key_map[key].key_code;
  #else
    return key_states[key].current;
  #endif
}
bool Input::key_up(Key key)
{
  #ifdef TARGET_3DS
    return !(~kDown & key_map[key].key_code);
  #else
    return !key_states[key].current;
  #endif
}

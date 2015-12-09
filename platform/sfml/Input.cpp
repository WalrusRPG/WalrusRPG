#include "Input.h"
#include "Graphics.h" // window
#include "sfwindow.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window.hpp>

#define INPUT WalrusRPG::Input
using WalrusRPG::Input::Key;
using WalrusRPG::Input::KeyState;
using sf::Keyboard;

struct InputMap
{
    Key key;
    sf::Keyboard::Key key_code;
    sf::Keyboard::Key key_code_alt;
};

KeyState key_states[Key::K_SIZE] = {KeyState::KS_RELEASED};
InputMap key_map[] = {
    {Key::K_A, Keyboard::W, Keyboard::Z},
    {Key::K_B, Keyboard::X, Keyboard::Unknown},
    {Key::K_L, Keyboard::Q, Keyboard::A},
    {Key::K_R, Keyboard::S, Keyboard::Unknown},

    {Key::K_UP, Keyboard::Up, Keyboard::Unknown},
    {Key::K_DOWN, Keyboard::Down, Keyboard::Unknown},
    {Key::K_LEFT, Keyboard::Left, Keyboard::Unknown},
    {Key::K_RIGHT, Keyboard::Right, Keyboard::Unknown},

    {Key::K_START, Keyboard::Return, Keyboard::Unknown},
    {Key::K_SELECT, Keyboard::BackSpace, Keyboard::Unknown},
};

void INPUT::key_poll()
{
    bool hasFocus = window.hasFocus();
    for (unsigned i = 0; i < K_SIZE; i++)
    {
        bool current_key_state = hasFocus && Keyboard::isKeyPressed(key_map[i].key_code);
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

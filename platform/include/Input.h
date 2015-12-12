#ifndef INCLUDE_INPUT_H
#define INCLUDE_INPUT_H

namespace WalrusRPG
{
    namespace Input
    {
        enum Key
        {
            K_A,
            K_B,
            K_L,
            K_R,
            K_UP,
            K_DOWN,
            K_LEFT,
            K_RIGHT,
            K_START,
            K_SELECT,
            K_SIZE
        };

        enum KeyState
        {
            KS_RELEASED,
            KS_JUST_RELEASED,
            KS_JUST_PRESSED,
            KS_PRESSED
        };


        void key_poll();

        KeyState key_get_state(Key key);
        bool key_pressed(Key key);
        bool key_released(Key key);
        bool key_down(Key key);
        bool key_up(Key key);
    }
}

#endif

#include "StateMachine.h"
#include "Logger.h"
#include "Timing.h"
#include "platform.h"
#include "Graphics.h"
#include "Status.h"
#include "render/Text.h"
#include "version.h"
#include "input/Input.h"
#include "utility/misc.h"

#if TARGET_SFML
#include <SFML/System.hpp>
#endif

using namespace WalrusRPG; /*::StateMachine*/
using namespace WalrusRPG::Graphics;
using namespace WalrusRPG::Timing;
using namespace WalrusRPG::Logger;
using WalrusRPG::Input::Key;
using WalrusRPG::Input::KeyState;
using WalrusRPG::States::State;

namespace
{
#if TARGET_SFML
    float update_times[200] = {0};
    float render_times[200] = {0};
    bool show_logger = false;
    bool show_state_debug = false;

    /**
     * Debug function showing a button state.
     */
    void draw_button(ImDrawList *list, signed x, signed y, KeyState state)
    {
        uint32_t button_color;
        switch (state)
        {
            case KeyState::KS_RELEASED:
                button_color = 0xFFFFFFFF;
                break;
            case KeyState::KS_JUST_RELEASED:
                button_color = 0xFFFFFF00;
                break;
            case KeyState::KS_JUST_PRESSED:
                button_color = 0xFFFF00FF;
                break;
            case KeyState::KS_PRESSED:
                button_color = 0xFF000000;
                break;
        }
        list->AddRectFilled({static_cast<float>(x), static_cast<float>(y)},
                            {static_cast<float>(x) + 8, static_cast<float>(y) + 8},
                            button_color);
    }

    /**
     * Draws WRPG's buttons states.
     */
    void draw_buttons()
    {
        ImGui::BeginGroup();
        ImGui::Text("Input state");
        ImGui::Indent();
        ImVec2 s = ImGui::GetCursorScreenPos();
        ImDrawList *list = ImGui::GetWindowDrawList();
        draw_button(list, s.x + 0, s.y, key_get_state(Key::K_L));
        draw_button(list, s.x + 56, s.y, key_get_state(Key::K_R));

        draw_button(list, s.x + 8, s.y + 8, key_get_state(Key::K_UP));
        draw_button(list, s.x + 0, s.y + 16, key_get_state(Key::K_LEFT));
        draw_button(list, s.x + 16, s.y + 16, key_get_state(Key::K_RIGHT));
        draw_button(list, s.x + 8, s.y + 24, key_get_state(Key::K_DOWN));

        draw_button(list, s.x + 27, s.y + 24, key_get_state(Key::K_SELECT));
        draw_button(list, s.x + 37, s.y + 24, key_get_state(Key::K_START));

        draw_button(list, s.x + 48, s.y + 20, key_get_state(Key::K_B));
        draw_button(list, s.x + 56, s.y + 12, key_get_state(Key::K_A));
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 32 +
                             ImGui::GetTextLineHeightWithSpacing());
        ImGui::Unindent();
        ImGui::EndGroup();
    }
#endif

    // State stack. Pointer because polymorphism.
    static tinystl::vector<WalrusRPG::States::State *> stack;

} /* namespace */

void StateMachine::init()
{
    log("State Machine init");
}

void StateMachine::deinit()
{
    log("State Machine deinit");
    while (!stack.empty())
    {
        delete stack.back();
        stack.pop_back();
    }
    stack.clear();
}

void StateMachine::push(State *state)
{
    stack.push_back(state);
}

void StateMachine::pop()
{
    // Mmmh, should StateMachine manage the state's destruction?...
    delete stack.back();
    stack.pop_back();
}

void StateMachine::run()
{
    const unsigned loop_time = TIMER_FREQ / 60;
    unsigned loop_next = loop_time;
    unsigned last_update = 0, update_stamp, update_time;
    unsigned last_frame = 0, frame_stamp, frame_time;

    // TODO : Better way to handle FPS while not breaking anything. There are some issues
    // if the update loop takes too much time.
    unsigned lag = 0;
    while (!stack.empty() && !Status::mustQuit())
    {
        Status::update();
        Input::key_poll();
        update_stamp = Timing::gettime();
        update_time = update_stamp - last_update;
        last_update = update_stamp;
        lag += update_time;

        while (lag >= loop_time)
        {
            stack.back()->update();
            lag -= loop_time;
        }

        if (Timing::gettime() < loop_next)
        {
            frame_stamp = Timing::gettime();
            frame_time = frame_stamp - last_frame;
            Graphics::frame_begin();
            // Update the current state
            stack.back()->render();
            last_frame = frame_stamp;

            // Text::print_format(0, 0, "WRPG build %s", git_version);
            if (frame_time != 0 && update_time != 0)
            {
                Text::print_format(0, 240 - 8, "%ufps, %uups", TIMER_FREQ / frame_time,
                                   TIMER_FREQ / update_time);
            }

#if TARGET_SFML
            // Updating time graph
            float update_time = (frame_stamp - update_stamp) / 1000.;
            float render_time = (Timing::gettime() - frame_stamp) / 1000.;
            for (int i = 0; i < 200; ++i)
            {
                update_times[i] = update_times[i + 1];
                render_times[i] = render_times[i + 1];
            }
            update_times[199] = update_time;
            render_times[199] = render_time;
            // Drawing FPS and render graphs
            ImGui::BeginGroup();
            ImGui::Text("FPS: %.2f", TIMER_FREQ / (float) frame_time);
            ImGui::PlotLines("", update_times, 200, 0, "Update");
            ImGui::SameLine();
            ImGui::Text("%.2fms", update_time);
            ImGui::PlotLines("", render_times, 200, 0, "Render");
            ImGui::SameLine();
            ImGui::Text("%.2fms", render_time);
            ImGui::EndGroup();

            ImGui::Separator();

            // Drawing Window Toggles
            ImGui::Text("Toggles");
            ImGui::Indent();
            ImGui::BeginGroup();
            ImGui::Checkbox("Logger", &show_logger);
            ImGui::Checkbox("State", &show_state_debug);
            ImGui::EndGroup();
            ImGui::Unindent();

            ImGui::Separator();

            // Drawing buttons
            draw_buttons();


            if (show_logger)
                Logger::debug_render();
            if (show_state_debug)
                stack.back()->debug(100 * frame_time / TIMER_FREQ);
#endif

            Graphics::frame_end();
        }

#ifdef ACTIVE_WAIT
        while (Timing::gettime() < loop_next)
            ;
#endif
        loop_next += loop_time;
    }
}

#pragma once

#include <SDL.h>
#include <functional>
#include <string>

#include "Button.hpp"
#include "sosim/SDL_Factory.hpp"
#include "GameObject.hpp"


namespace scene::ui
{
    class WinPopup : public GameObject
    {
    public:
        WinPopup(SDL_Renderer*);

        void handle_event(const SDL_Event&) override;
        void update(const chrono::milliseconds&) override;
        void render(SDL_Renderer*) const override;

        void set_enabled(bool enable) { m_isEnabled = enable; }
        bool get_enabled() const { return m_isEnabled; }

        void set_position(bool isLeftWin);
        void set_on_replay_button_click(std::function<void()> f) { m_replayButton.set_on_mouse_click(f); }
        void set_on_title_screen_button_click(std::function<void()> f) { m_titleScreenButton.set_on_mouse_click(f); }
        void set_on_quit_game_button_click(std::function<void()> f) { m_quitGameButton.set_on_mouse_click(f); }

    private:
        static constexpr SDL_Point LEFT_WIN_POSITION {40, 120};
        static constexpr SDL_Point RIGHT_WIN_POSITION {640-40-270, 120};

        static constexpr SDL_Rect WIN_TEXT_RELATIVE_TRANSFORM {60, 0, 100, 40};
        static constexpr SDL_Rect REPLAY_BUTTON_RELATIVE_TRANSFORM {0, 60, 270, 60};
        static constexpr SDL_Rect TITLE_SCREEN_BUTTON_RELATIVE_TRANSFORM {0, 140, 270, 40};
        static constexpr SDL_Rect QUIT_GAME_BUTTON_RELATIVE_TRANSFORM {0, 190, 270, 40};

        static constexpr std::u16string_view REPLAY_BUTTON_TEXT = u"PLAY AGAIN";
        static constexpr std::u16string_view TITLE_SCREEN_BUTTON_TEXT = u"MAIN MENU";
        static constexpr std::u16string_view QUIT_GAME_BUTTON_TEXT = u"QUIT GAME";
        static constexpr std::u16string_view WIN_TEXT = u"WIN";

        SDL_Point m_position;
        bool m_isEnabled = false;

        SDL_Rect m_winTextTransform;
        sosim::u_ptr<SDL_Texture> m_winText;
        Button m_replayButton;
        Button m_titleScreenButton;
        Button m_quitGameButton;
    };
}
#include "scene/ui/WinPopup.hpp"
#include "asset/AssetPackage.hpp"

#include <cassert>

using namespace asset;


namespace scene::ui
{
    WinPopup::WinPopup(SDL_Renderer* renderer)
        : m_replayButton(std::u16string(REPLAY_BUTTON_TEXT), AssetPackage::get_instance()->m_d2coding.get(), renderer, REPLAY_BUTTON_RELATIVE_TRANSFORM),
        m_titleScreenButton(std::u16string(TITLE_SCREEN_BUTTON_TEXT), AssetPackage::get_instance()->m_d2coding.get(), renderer, TITLE_SCREEN_BUTTON_RELATIVE_TRANSFORM),
        m_quitGameButton(std::u16string(QUIT_GAME_BUTTON_TEXT), AssetPackage::get_instance()->m_d2coding.get(), renderer, QUIT_GAME_BUTTON_RELATIVE_TRANSFORM)
    {
        m_winText = sosim::make_texture_from_text(renderer, AssetPackage::get_instance()->m_d2coding.get(), std::u16string(WIN_TEXT), {255,255,255,255});
        set_position(true);
    }

    void WinPopup::handle_event(const SDL_Event& e)
    {
        if (!m_isEnabled)
            return;

        switch (e.type)
        {
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
        case SDL_EventType::SDL_MOUSEBUTTONUP:
        case SDL_EventType::SDL_MOUSEMOTION:
            m_replayButton.handle_event(e);
            m_titleScreenButton.handle_event(e);
            m_quitGameButton.handle_event(e);
            break;
        }
    }

    void WinPopup::update(const chrono::milliseconds& deltaTime)
    {
        if (!m_isEnabled)
            return;

        m_replayButton.update(deltaTime);
        m_titleScreenButton.update(deltaTime);
        m_quitGameButton.update(deltaTime);
    }

    void WinPopup::render(SDL_Renderer* renderer) const
    {
        if (!m_isEnabled)
            return;
        
        assert((SDL_RenderCopy(renderer, m_winText.get(), nullptr, &m_winTextTransform) == 0));
        m_replayButton.render(renderer);
        m_titleScreenButton.render(renderer);
        m_quitGameButton.render(renderer);
    }

    SDL_Rect operator + (SDL_Point p, SDL_Rect r)
    {
        return {p.x + r.x, p.y + r.y, r.w, r.h };
    }

    void WinPopup::set_position(bool isLeftWin)
    {
        if (isLeftWin)
        {
            m_position = LEFT_WIN_POSITION;
        }
        else
        {
            m_position = RIGHT_WIN_POSITION;
        }

        m_winTextTransform = m_position + WIN_TEXT_RELATIVE_TRANSFORM;
        m_replayButton.set_transform(m_position + REPLAY_BUTTON_RELATIVE_TRANSFORM);
        m_titleScreenButton.set_transform(m_position + TITLE_SCREEN_BUTTON_RELATIVE_TRANSFORM);
        m_quitGameButton.set_transform(m_position + QUIT_GAME_BUTTON_RELATIVE_TRANSFORM);
    }
}
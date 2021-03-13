#include "scene/SelectPlayer.hpp"

#include "asset/AssetPackage.hpp"
#include <exception>
using namespace asset;


namespace scene
{
    SelectPlayer::SelectPlayer(SDL_Renderer* renderer)
        : m_pve(PVE_STR, AssetPackage::get_instance()->m_d2coding.get(), renderer, {200, 192, 240, 40}),
        m_pvp(PVP_STR, AssetPackage::get_instance()->m_d2coding.get(), renderer, {200, 247, 240, 40})
    {
        m_pve.set_on_mouse_click([this](std::u16string text){ this->c_on_button_click(text); });
        m_pvp.set_on_mouse_click([this](std::u16string text){ this->c_on_button_click(text); });
    }

    void SelectPlayer::handle_events()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EventType::SDL_QUIT:
                m_isQuit = true;
                break;
            
            case SDL_EventType::SDL_MOUSEMOTION:
            case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            case SDL_EventType::SDL_MOUSEBUTTONUP:
                m_pve.handle_event(e);
                m_pvp.handle_event(e);
                break;
            }
        }
    }

    void SelectPlayer::update(const chrono::milliseconds& deltaTime)
    {
        
    }

    void SelectPlayer::render(SDL_Renderer* const renderer)
    {
        m_pve.render(renderer);
        m_pvp.render(renderer);
    }

    void SelectPlayer::c_on_button_click(std::u16string buttonText)
    {
        if (buttonText == PVE_STR)
        {
            m_nextScene = SceneType::PLAY_PVE;
        }
        else if (buttonText == PVP_STR)
        {
            m_nextScene = SceneType::PLAY_PVP;
        }
        else
        {
            throw std::invalid_argument("invalid buttonText");
        }
    }
}
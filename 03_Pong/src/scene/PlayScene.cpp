#include "scene/PlayScene.hpp"
#include "asset/AssetPackage.hpp"


namespace scene
{
    PlayScene::PlayScene(bool isPVP)
        : m_isPVP(isPVP)
    {

    }

    void PlayScene::handle_events()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_EventType::SDL_QUIT:
                m_isQuit = true;
                break;
            }
        }
    }

    void PlayScene::update(const chrono::milliseconds& deltaTime)
    {
        // TODO: Implement keyboard move

        // TODO: Implement ball move

        // TODO: Goal handle (ball reset, update score..)

    }

    void PlayScene::render(SDL_Renderer* const renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        SDL_RenderFillRect(renderer, &m_ball);
        SDL_RenderFillRect(renderer, &m_leftPlayer);
        SDL_RenderFillRect(renderer, &m_rightPlayer);

        SDL_RenderDrawLine(renderer, 320, 0, 320, 480);

        render_scores(renderer);
    }

    void PlayScene::render_scores(SDL_Renderer* const renderer)
    {
        if (m_isLeftScoreUpdated)
        {
            m_leftScoreText = sosim::make_texture_from_text(renderer, asset::AssetPackage::get_instance()->m_d2coding.get(), std::to_string(m_leftScore), {255, 255, 255, 255});
            int w, h;
            SDL_QueryTexture(m_leftScoreText.get(), nullptr, nullptr, &w, &h);
            m_leftScoreTextPos = {320-35-w, 30, w, h};
            
            m_isLeftScoreUpdated = false;
        }
        if (m_isRightScoreUpdated)
        {
            m_rightScoreText = sosim::make_texture_from_text(renderer, asset::AssetPackage::get_instance()->m_d2coding.get(), std::to_string(m_rightScore), {255, 255, 255, 255});
            int w, h;
            SDL_QueryTexture(m_leftScoreText.get(), nullptr, nullptr, &w, &h);
            m_rightScoreTextPos = {320+35, 30, w, h};

            m_isRightScoreUpdated = false;
        }

        SDL_RenderCopy(renderer, m_leftScoreText.get(), nullptr, &m_leftScoreTextPos);
        SDL_RenderCopy(renderer, m_rightScoreText.get(), nullptr, &m_rightScoreTextPos);
    }
}
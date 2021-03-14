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
        update_right_player_move(deltaTime);
        if (m_isPVP)
        {
            update_left_player_move(deltaTime);
        }
        else
        {
            update_left_cpu_move(deltaTime);
        }

        update_ball_move(deltaTime);

        Team goalTeam = check_goal();
        if (goalTeam != NONE)
        {
            process_goal(goalTeam);
        }
    }

    void PlayScene::render(SDL_Renderer* const renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        SDL_RenderFillRectF(renderer, &m_ball);
        SDL_RenderFillRectF(renderer, &m_leftBar);
        SDL_RenderFillRectF(renderer, &m_rightBar);

        SDL_RenderDrawLine(renderer, 320, 0, 320, 480);

        render_scores(renderer);
    }

    void PlayScene::update_right_player_move(const chrono::milliseconds& deltaTime)
    {
        const bool* keyStates = reinterpret_cast<const bool*>(SDL_GetKeyboardState(nullptr));
        if (keyStates[SDL_SCANCODE_UP] && !keyStates[SDL_SCANCODE_DOWN])
        {
            m_rightBar.y -= BAR_SPEED * deltaTime.count();
            if (m_rightBar.y < 0)
            {
                m_rightBar.y = 0;
            }
        }
        if (keyStates[SDL_SCANCODE_DOWN] && !keyStates[SDL_SCANCODE_UP])
        {
            m_rightBar.y += BAR_SPEED * deltaTime.count();
            if (m_rightBar.y + m_rightBar.h > 480)
            {
                m_rightBar.y = 480 - m_rightBar.h;
            }
        }
    }

    void PlayScene::update_left_player_move(const chrono::milliseconds& deltaTime)
    {
        const bool* keyStates = reinterpret_cast<const bool*>(SDL_GetKeyboardState(nullptr));
        if (keyStates[SDL_SCANCODE_W] && !keyStates[SDL_SCANCODE_S])
        {
            m_leftBar.y -= BAR_SPEED * deltaTime.count();
            if (m_leftBar.y < 0)
            {
                m_leftBar.y = 0;
            }
        }
        if (keyStates[SDL_SCANCODE_S] && !keyStates[SDL_SCANCODE_W])
        {
            m_leftBar.y += BAR_SPEED * deltaTime.count();
            if (m_leftBar.y + m_leftBar.h > 480)
            {
                m_leftBar.y = 480 - m_leftBar.h;
            }
        }
    }

    void PlayScene::update_left_cpu_move(const chrono::milliseconds& deltaTime)
    {
        // TODO: Implement cpu move
    }

    void PlayScene::update_ball_move(const chrono::milliseconds& deltaTime)
    {
        // TODO: Implement ball move
    }

    PlayScene::Team PlayScene::check_goal()
    {
        // TODO: Implement check goal
        return Team::NONE;
    }

    void PlayScene::process_goal(Team goalTeam)
    {
        // TODO: Goal handle (ball reset, update score..)
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
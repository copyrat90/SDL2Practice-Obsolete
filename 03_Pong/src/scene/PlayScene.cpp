#include "scene/PlayScene.hpp"
#include "asset/AssetPackage.hpp"

#include <cmath>
#include <cassert>

#ifndef NDEBUG
#include <iostream>
#endif


namespace scene
{
    PlayScene::PlayScene(SDL_Renderer* renderer, bool isPVP)
        : m_winPopup(renderer), m_isPVP(isPVP)
    {
        m_winPopup.set_on_replay_button_click([this]{this->restart_game();});
        m_winPopup.set_on_title_screen_button_click([this]{this->m_nextScene = SceneType::SELECT_PLAYER; });
        m_winPopup.set_on_quit_game_button_click([this] {this->m_isQuit = true; });
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
            case SDL_EventType::SDL_MOUSEBUTTONDOWN:
            case SDL_EventType::SDL_MOUSEBUTTONUP:
            case SDL_EventType::SDL_MOUSEMOTION:
                m_winPopup.handle_event(e);
                break;
            }
        }
    }

    void PlayScene::update(const chrono::milliseconds& deltaTime)
    {
        update_game(deltaTime);
        update_ui(deltaTime);
    }

    void PlayScene::update_game(const chrono::milliseconds& deltaTime)
    {
        if (!m_isGameOngoing)
        {
            return;
        }

        update_ball_move(deltaTime);
        
        update_right_player_move(deltaTime);
        if (m_isPVP)
        {
            update_left_player_move(deltaTime);
        }
        else
        {
            update_left_cpu_move(deltaTime);
        }

        Team goalTeam = check_goal();
        if (goalTeam != NONE)
        {
            process_goal(goalTeam);
        }

        Team winTeam = check_win();
        if (winTeam != NONE)
        {
            process_win(winTeam);
        }
    }

    void PlayScene::update_ui(const chrono::milliseconds& deltaTime)
    {
        m_winPopup.update(deltaTime);
    }

    void PlayScene::render(SDL_Renderer* const renderer)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        SDL_RenderFillRectF(renderer, &m_ball);
        SDL_RenderFillRectF(renderer, &m_leftBar);
        SDL_RenderFillRectF(renderer, &m_rightBar);

        SDL_RenderDrawLine(renderer, 320, 0, 320, 480);

        render_scores(renderer);

        m_winPopup.render(renderer);
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
        const float ballPosY = m_ball.y + m_ball.h / 2;
        const float barPosY = m_leftBar.y + m_leftBar.h / 2;
        
        int cpuMoveDirection = 0;
        if (std::abs(barPosY - ballPosY) < 5)
        {
            cpuMoveDirection = 0;
        }
        else if (barPosY > ballPosY)
        {
            cpuMoveDirection = -1;
        }
        else
        {
            cpuMoveDirection = 1;
        }
        m_leftBar.y += cpuMoveDirection * BAR_SPEED * deltaTime.count();
    }

    void PlayScene::update_ball_move(const chrono::milliseconds& deltaTime)
    {
        // on serving
        if (m_serveDelayTime > 0ms)
        {
            m_serveDelayTime -= deltaTime;
            if (m_serveDelayTime <= 0ms)
            {
                int serveDirection = (m_serveTeam == Team::LEFT) ? -1 : 1;
                m_ballVelocity = {serveDirection * BALL_SERVE_SPEED, m_ballServeYVelRange(m_RNG)};
            }
        }
        // not on serving
        else
        {
            m_ball.x += m_ballVelocity.x * deltaTime.count();
            m_ball.y += m_ballVelocity.y * deltaTime.count();

            if (is_ball_collide_with_bar(m_leftBar))
            {
                // TODO
            }
            else if (is_ball_collide_with_bar(m_rightBar))
            {
                // TODO
            }
            if (is_ball_collide_with_ceiling())
            {
                m_ballVelocity.y = -m_ballVelocity.y;
                m_ball.y = -m_ball.y;
            }
            else if (is_ball_collide_with_floor())
            {
                m_ballVelocity.y = -m_ballVelocity.y;
                float offDistance = m_ball.y + m_ball.h - 480;
                m_ball.y -= 2 * offDistance;
            }
            Team goalTeam = check_goal();
            if (goalTeam != Team::NONE)
            {
                process_goal(goalTeam);
            }
        }
    }

    inline bool PlayScene::is_ball_collide_with_ceiling() const
    {
        return m_ball.y <= 0;
    }

    inline bool PlayScene::is_ball_collide_with_floor() const
    {
        return m_ball.y + m_ball.h >= 480;
    }

    inline bool PlayScene::is_ball_collide_with_bar(const SDL_FRect& bar) const
    {
        if (m_ball.x + m_ball.w < bar.x)
            return false;
        if (bar.x + bar.w < m_ball.x)
            return false;
        if (m_ball.y + m_ball.h < bar.y)
            return false;
        if (bar.y + bar.h < m_ball.y)
            return false;
        
        return true;
    }

    PlayScene::Team PlayScene::check_goal()
    {
        if (m_ball.x < -m_ball.w)
            return Team::RIGHT;
        if (m_ball.x > 640 + m_ball.w)
            return Team::LEFT;
        
        return Team::NONE;
    }

    void PlayScene::process_goal(Team goalTeam)
    {
        add_score(goalTeam);
        reset_ball();
    }
    
    void PlayScene::add_score(Team goalTeam)
    {
        if (goalTeam == Team::LEFT)
        {
            ++m_leftScore;
            m_isLeftScoreUpdated = true;
        }
        else if (goalTeam == Team::RIGHT)
        {
            ++m_rightScore;
            m_isRightScoreUpdated = true;
        }
        else
        {
            assert((false && "tried to add score to Team::NONE"));
        }
    }
    
    void PlayScene::reset_ball()
    {
        m_ball.x = 313;
        m_ball.y = 233;
        m_serveTeam = (m_serveTeam == LEFT) ? RIGHT : LEFT;
        m_serveDelayTime = SERVE_DELAY;
    }

    PlayScene::Team PlayScene::check_win()
    {
        if (m_leftScore >= GAME_SET_SCORE)
            return Team::LEFT;
        if (m_rightScore >= GAME_SET_SCORE)
            return Team::RIGHT;
        
        return Team::NONE;
    }

    void PlayScene::process_win(Team winner)
    {
        assert((winner != Team::NONE && "win team is Team::NONE"));

        m_isGameOngoing = false;
        m_winPopup.set_position(winner == Team::LEFT);
        m_winPopup.set_enabled(true);
    }

    void PlayScene::restart_game()
    {
        m_winPopup.set_enabled(false);

        m_leftScore = 0;
        m_rightScore = 0;
        m_isLeftScoreUpdated = true;
        m_isRightScoreUpdated = true;

        m_leftBar = {25, 203, 15, 75};
        m_rightBar = {600, 203, 15, 75};
        m_ball = {313, 233, 15, 15};

        m_serveTeam = Team::LEFT;
        m_serveDelayTime = SERVE_DELAY;

        m_isGameOngoing = true;
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
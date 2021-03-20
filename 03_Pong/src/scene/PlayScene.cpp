#include "scene/PlayScene.hpp"
#include "asset/AssetPackage.hpp"


#include <cassert>
#include <limits>

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
            SDL_FRect prevBall = m_ball;
            m_ball.x += m_ballVelocity.x * deltaTime.count();
            m_ball.y += m_ballVelocity.y * deltaTime.count();

            update_ball_collide_with_bar(m_rightBar, prevBall);
            update_ball_collide_with_bar(m_leftBar, prevBall);

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

    int ccw(const SDL_FPoint& p1, const SDL_FPoint& p2, const SDL_FPoint& p3)
    {
        const SDL_FPoint v1 { p2.x - p1.x, p2.y - p1.y };
        const SDL_FPoint v2 { p3.x - p1.x, p3.y - p1.y };

        const float cross = v1.x * v2.y - v2.x * v1.y;
        if (std::abs(cross) <= FLT_EPSILON)
            return 0;
        return (cross < 0) ? -1 : +1;
    }

    enum class Intersection { NONE, POINT, LINE };
    Intersection check_two_line_intersect(std::pair<SDL_FPoint, SDL_FPoint> line1, std::pair<SDL_FPoint, SDL_FPoint> line2)
    {
        const int ccw1 = ccw(line1.first, line1.second, line2.first);
        const int ccw2 = ccw(line1.first, line1.second, line2.second);
        const int ccwa = ccw(line2.first, line2.second, line1.first);
        const int ccwb = ccw(line2.first, line2.second, line1.second);

        if (0 == ccw1 && 0 == ccw2 && 0 == ccwa && 0 == ccwb)
        {
            auto sortLinePoint = [] (std::pair<SDL_FPoint,SDL_FPoint>& line)
            {
                if (std::abs(line.first.x - line.second.x) <= FLT_EPSILON)
                {
                    std::swap(line.first.x, line.first.y);
                    std::swap(line.second.x, line.second.y);
                }

                if (line.first.x > line.second.x)
                {
                    std::swap(line.first, line.second);
                }
            };

            sortLinePoint(line1);
            sortLinePoint(line2);
            
            if (std::abs(line1.second.x - line2.first.x) <= FLT_EPSILON ||
                std::abs(line2.second.x - line1.first.x) <= FLT_EPSILON)
                return Intersection::POINT;
            else if (line1.second.x > line2.first.x && line1.first.x < line2.second.x)
                return Intersection::LINE;
            else
                return Intersection::NONE;
        }
        else
        {
            if (ccw1 * ccw2 <= 0 && ccwa * ccwb <= 0)
                return Intersection::POINT;
            else
                return Intersection::NONE;
        }
    }

    SDL_FPoint get_line_intersect_point(const std::pair<SDL_FPoint,SDL_FPoint>& line1, const std::pair<SDL_FPoint,SDL_FPoint>& line2)
    {
        const float& x1 = line1.first.x;    const float& y1 = line1.first.y;
        const float& x2 = line1.second.x;   const float& y2 = line1.second.y;
        const float& x3 = line2.first.x;    const float& y3 = line2.first.y;
        const float& x4 = line2.second.x;   const float& y4 = line2.second.y;

        const float det = (y2-y1)*(x4-x3) - (x2-x1)*(y4-y3);

        if (std::abs(det) <= FLT_EPSILON)
            throw std::logic_error("determinant is zero");
        
        // Cramer's rule
        const float x = ((x1*y2-x2*y1)*(x4-x3) - (x2-x1)*(x3*y4-x4*y3)) / det;
        const float y = (((y2-y1)*(x3*x4-x4*y3)) - (x1*y2-x2*y1)*(y4-y3)) / det;
        return {x, y};
    }

    void PlayScene::update_ball_collide_with_bar(const SDL_FRect& bar, const SDL_FRect& prevBall)
    {
        const SDL_FPoint displacement { m_ball.x - prevBall.x, m_ball.y - prevBall.y };

        const SDL_FPoint leftUpper { prevBall.x, prevBall.y };
        const SDL_FPoint rightUpper { prevBall.x + prevBall.w, prevBall.y };
        const SDL_FPoint leftLower { prevBall.x, prevBall.y + prevBall.h };
        const SDL_FPoint rightLower { prevBall.x + prevBall.w, prevBall.y + prevBall.h };

        auto addVectorToPoint = [] (const SDL_FPoint& point, const SDL_FPoint& vector) -> SDL_FPoint
        {
            return { point.x + vector.x, point.y + vector.y };
        };

        std::pair<SDL_FPoint,SDL_FPoint> displacementLines[4]
        {
            { leftUpper, addVectorToPoint(leftUpper, displacement) },
            { rightUpper, addVectorToPoint(rightUpper, displacement) },
            { leftLower, addVectorToPoint(leftLower, displacement) },
            { rightLower, addVectorToPoint(rightLower, displacement) }
        };

        std::pair<SDL_FPoint,SDL_FPoint> barLines[4]
        {
            { { bar.x, bar.y }, { bar.x + bar.w, bar.y} },
            { { bar.x + bar.w, bar.y}, { bar.x + bar.w, bar.y + bar.h } },
            { { bar.x, bar.y + bar.h }, { bar.x + bar.w, bar.y + bar.h } },
            { { bar.x, bar.y }, { bar.x, bar.y + bar.h } },
        };

        auto distanceSquared = [] (std::pair<SDL_FPoint,SDL_FPoint> line) -> double
        {
            return std::pow((line.second.x - line.first.x), 2) + std::pow((line.second.y - line.first.y), 2);
        };

        int collideDisplacementLineIdx = -1, collideBarLineIdx = -1;
        double minDistanceSquared = std::numeric_limits<double>::infinity();
        Intersection intersect = Intersection::NONE;
        SDL_FPoint intersectPoint = {-1,-1};
        
        // 최저거리 충돌점 구하기
        for (int disIdx = 0; disIdx < 4; ++disIdx)
        {
            for (int barIdx = 0; barIdx < 4; ++barIdx)
            {
                const auto& disLine = displacementLines[disIdx];
                const auto& barLine = barLines[barIdx];

                switch (check_two_line_intersect(disLine, barLine))
                {
                case Intersection::NONE:
                    break;
                case Intersection::POINT:
                    intersect = Intersection::POINT;

                    // 교점 구하고 거리 구해서 최저거리일 경우 충돌점(+idx) 업데이트
                    SDL_FPoint candidateIntersectPoint = get_line_intersect_point(disLine, barLine);
                    double candidateDistance = distanceSquared({disLine.first, candidateIntersectPoint});
                    if (candidateDistance < minDistanceSquared)
                    {
                        minDistanceSquared = candidateDistance;
                        intersectPoint = candidateIntersectPoint;
                        collideDisplacementLineIdx = disIdx;
                        collideBarLineIdx = barIdx;
                    }
                    break;
                case Intersection::LINE:
                    intersect = Intersection::LINE;
                    goto DOUBLE_LOOP_EXIT;
                }
            }
        }
    DOUBLE_LOOP_EXIT:
        // TODO: 충돌점 바탕으로 충돌 처리
        switch (intersect)
        {
        case Intersection::NONE:
            break;
        case Intersection::POINT:
            break;
        case Intersection::LINE:
        }

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
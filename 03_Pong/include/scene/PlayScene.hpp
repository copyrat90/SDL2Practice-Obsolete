#ifndef PLAY_SCENE_HPP
#define PLAY_SCENE_HPP

#include "Scene.hpp"

#include "sosim/SDL_Factory.hpp"

#include "scene/ui/WinPopup.hpp"

#include <random>

using namespace std::chrono_literals;
using namespace scene;


namespace scene
{
    class PlayScene : public Scene
    {
    public:
        PlayScene() = delete;
        PlayScene(SDL_Renderer*, bool isPVP);
        
        void handle_events() override;
        void update(const chrono::milliseconds&) override;
        void render(SDL_Renderer* const) override;

    private:
        enum Team { NONE, LEFT, RIGHT};

        static constexpr float BAR_SPEED = 0.6f;
        static constexpr float BALL_REGULAR_SPEED = 0.6f;
        static constexpr float BALL_SERVE_SPEED = 0.3f;
        static constexpr float BALL_MAX_Y_VEL = 0.8f;
        static constexpr int GAME_SET_SCORE = 10;

        bool m_isGameOngoing = true;

        int m_leftScore = 0;
        int m_rightScore = 0;

        sosim::u_ptr<SDL_Texture> m_leftScoreText = nullptr;
        sosim::u_ptr<SDL_Texture> m_rightScoreText = nullptr;
        SDL_Rect m_leftScoreTextPos {};
        SDL_Rect m_rightScoreTextPos {};
        bool m_isLeftScoreUpdated = true;
        bool m_isRightScoreUpdated = true;

        SDL_FRect m_leftBar {25, 203, 15, 75};
        SDL_FRect m_rightBar {600, 203, 15, 75};
        SDL_FRect m_ball {313, 233, 15, 15};

        ui::WinPopup m_winPopup;

        SDL_FPoint m_ballVelocity {};
        Team m_serveTeam = Team::LEFT;

        bool m_isPVP;

        static constexpr chrono::milliseconds SERVE_DELAY = 1500ms;
        chrono::microseconds m_serveDelayTime = SERVE_DELAY;

        std::mt19937 m_RNG {std::random_device()()};
        std::uniform_real_distribution<float> m_ballServeYVelRange {-0.2f, 0.2f};

        void update_game(const chrono::milliseconds&);
        void update_ui(const chrono::milliseconds&);

        void update_right_player_move(const chrono::milliseconds&);
        void update_left_player_move(const chrono::milliseconds&);
        void update_left_cpu_move(const chrono::milliseconds&);

        void update_ball_move(const chrono::milliseconds&);

        inline bool is_ball_collide_with_ceiling() const;
        inline bool is_ball_collide_with_floor() const;

        void update_ball_collide_with_bar(const SDL_FRect& bar, const SDL_FRect& prevBall);

        Team check_goal();
        void process_goal(Team);
        void add_score(Team);
        void reset_ball();

        Team check_win();
        void process_win(Team);

        void restart_game();

        void render_scores(SDL_Renderer* const);
    };
}


#endif
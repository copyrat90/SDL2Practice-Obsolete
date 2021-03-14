#ifndef PLAY_SCENE_HPP
#define PLAY_SCENE_HPP

#include "Scene.hpp"

#include "sosim/SDL_Factory.hpp"


namespace scene
{
    class PlayScene : public Scene
    {
    public:
        PlayScene() = delete;
        PlayScene(bool isPVP);
        
        void handle_events() override;
        void update(const chrono::milliseconds&) override;
        void render(SDL_Renderer* const) override;

    private:
        enum Team { NONE, LEFT, RIGHT};

        static constexpr float BAR_SPEED = 0.6f;
        static constexpr float BALL_REGULAR_SPEED = 0.6f;
        static constexpr float BALL_SERVE_SPEED = 0.3f;
        static constexpr int GAME_SET_SCORE = 10;

        int m_leftScore = 0;
        int m_rightScore = 0;

        sosim::u_ptr<SDL_Texture> m_leftScoreText;
        sosim::u_ptr<SDL_Texture> m_rightScoreText;
        SDL_Rect m_leftScoreTextPos {};
        SDL_Rect m_rightScoreTextPos {};
        bool m_isLeftScoreUpdated = true;
        bool m_isRightScoreUpdated = true;

        SDL_FRect m_leftBar {25, 203, 15, 75};
        SDL_FRect m_rightBar {600, 203, 15, 75};
        SDL_FRect m_ball {313, 233, 15, 15};

        bool m_isPVP;

        void update_right_player_move(const chrono::milliseconds&);
        void update_left_player_move(const chrono::milliseconds&);
        void update_left_cpu_move(const chrono::milliseconds&);

        void update_ball_move(const chrono::milliseconds&);

        Team check_goal();
        void process_goal(Team);

        void render_scores(SDL_Renderer* const);
    };
}


#endif
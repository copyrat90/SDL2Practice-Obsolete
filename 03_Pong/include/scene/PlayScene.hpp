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

        static constexpr int GAME_SET_SCORE = 10;

        int m_leftScore = 0;
        int m_rightScore = 0;

        sosim::u_ptr<SDL_Texture> m_leftScoreText;
        sosim::u_ptr<SDL_Texture> m_rightScoreText;
        SDL_Rect m_leftScoreTextPos {};
        SDL_Rect m_rightScoreTextPos {};
        bool m_isLeftScoreUpdated = true;
        bool m_isRightScoreUpdated = true;

        SDL_Rect m_leftPlayer {25, 203, 15, 75};
        SDL_Rect m_rightPlayer {600, 203, 15, 75};
        SDL_Rect m_ball {313, 233, 15, 15};

        bool m_isPVP;

        void render_scores(SDL_Renderer* const);
    };
}


#endif
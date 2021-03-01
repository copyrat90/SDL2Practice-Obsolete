#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "sosim/SDL_Factory.hpp"

#include <string>
#include <chrono>

namespace chrono = std::chrono;
using namespace std::chrono_literals;


class Game
{
public:
    Game(std::string title, Uint32 fps = 60, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED, int w = 640, int h = 480, Uint32 windowFlags = SDL_WINDOW_OPENGL);
    ~Game();

    void run();
    
    bool init();
    void clean();

private:
    sosim::u_ptr<SDL_Window> m_window;
    sosim::u_ptr<SDL_Renderer> m_renderer;
	
	static constexpr float PLAYER_SPEED = 0.7f;
	
	SDL_FRect m_playerTransform {200, 200, 20, 20};
	SDL_FPoint m_playerVelocity {0, 0};

    Uint32 m_fps;
    
    bool m_ready;
    bool m_running;

    std::string m_title;
    int m_x, m_y, m_w, m_h;
    Uint32 m_windowFlags;

    void handle_events();
    void update(chrono::milliseconds deltaTime);
    void render();
};

#endif

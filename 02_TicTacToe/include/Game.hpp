#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "sosim/SDL_Factory.hpp"
#include "Board.hpp"


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

    Uint32 m_fps;
    
    bool m_ready;
    bool m_running;

    std::string m_title;
    int m_x, m_y, m_w, m_h;
    Uint32 m_windowFlags;

    Board m_board;

    void handle_events();
    void update(Uint32 deltaTicks);
    void render();

    void on_piece_changed(int y, int x);
};

#endif

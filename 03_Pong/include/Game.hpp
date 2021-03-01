#ifndef GAME_HPP
#define GAME_HPP

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "sosim/SDL_Factory.hpp"

#include <string>
#include <chrono>

#include "scene/SelectPlayer.hpp"

namespace chrono = std::chrono;
using namespace std::chrono_literals;
using namespace scene;


class Game
{
public:
    Game(std::string title, Uint32 fps = 60, int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED, int w = 640, int h = 480, Uint32 windowFlags = SDL_WINDOW_OPENGL);
    ~Game();

    void run();
    
    bool setup();
    void clean();

private:
    sosim::u_ptr<SDL_Window> m_window;
    sosim::u_ptr<SDL_Renderer> m_renderer;

    Uint32 m_fps;
    
    bool m_ready;
    bool m_isQuit;

    std::string m_title;
    int m_x, m_y, m_w, m_h;
    Uint32 m_windowFlags;

    std::unique_ptr<Scene> m_currentScene;
    std::unique_ptr<Scene> m_nextScene;

    SceneType m_nextSceneType;

    bool init();
    bool load_media();

    void handle_events();
    void update(chrono::milliseconds deltaTime);
    void render();

    void fetch_quit();
    void fetch_next_scene();
    void change_scene();
};

#endif

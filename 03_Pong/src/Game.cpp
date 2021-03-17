#include "Game.hpp"
#include <thread>
#include <exception>

#include "asset/AssetPackage.hpp"

#include "scene/SelectPlayer.hpp"
#include "scene/PlayScene.hpp"


Game::Game(std::string title, Uint32 fps, int x, int y, int w, int h, Uint32 windowFlags)
    : m_window(nullptr, sosim::SDL_Deleter()), m_renderer(nullptr, sosim::SDL_Deleter())
{
    m_ready = false;
    m_isQuit = false;

    m_fps = fps;
    m_title = title;
    m_x = x;
    m_y = y;
    m_w = w;
    m_h = h;
    m_windowFlags = windowFlags;
}

Game::~Game()
{
    clean();
}

void Game::run()
{
    if (!m_ready && !init())
    {
        printf("Game::init() failed\n");
        return;
    }

    m_currentScene = std::make_unique<SelectPlayer>(m_renderer.get());

    // Manage two seperate deltaTime for delaying & updating
    auto loopStartPoint = chrono::high_resolution_clock::now();
    auto updateStartPoint = chrono::high_resolution_clock::now();
    chrono::nanoseconds loopDeltaTime;
    chrono::nanoseconds updateDeltaTime;

    while (!m_isQuit)
    {
        loopStartPoint = chrono::high_resolution_clock::now();

        handle_events();

        updateDeltaTime = chrono::high_resolution_clock::now() - updateStartPoint;
        updateStartPoint = chrono::high_resolution_clock::now();

        update(chrono::duration_cast<chrono::milliseconds>(updateDeltaTime));

        fetch_quit();
        fetch_next_scene();
        change_scene();

        render();

        loopDeltaTime = chrono::high_resolution_clock::now() - loopStartPoint;
        const chrono::nanoseconds NS_PER_FRAME = 1000000000ns / m_fps;

        if (loopDeltaTime < NS_PER_FRAME)
        {
            const auto NS_TO_DELAY = NS_PER_FRAME - loopDeltaTime;
            std::this_thread::sleep_for(NS_TO_DELAY);
        }
    }
}

bool Game::setup()
{
    m_ready = init() && load_media();
    return m_ready;
}

void Game::clean()
{
    m_renderer.reset();
    m_window.reset();
    m_ready = false;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL_Init() failed : %s\n", SDL_GetError());
        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        printf("IMG_Init() failed : %s", IMG_GetError());
        return false;
    }

    if (TTF_Init() < 0)
    {
        printf("TTF_Init() error : %s", TTF_GetError());
        return false;
    }

    m_window = sosim::make_window(m_title.c_str(), m_x, m_y, m_w, m_h, m_windowFlags);
    if (!m_window)
    {
        printf("SDL_CreateWindow() failed : %s\n", SDL_GetError());
        return false;
    }

    m_renderer = sosim::make_renderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        printf("SDL_CreateRenderer() failed : %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool Game::load_media()
{
    auto& d2coding = asset::AssetPackage::get_instance()->m_d2coding = sosim::make_font("asset/font/D2Coding.ttf", 48);
    if (!d2coding)
        return false;

    return true;
}

void Game::handle_events()
{
    m_currentScene->handle_events();
}

void Game::update(chrono::milliseconds deltaTime)
{
    m_currentScene->update(deltaTime);
}

void Game::render()
{
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    m_currentScene->render(m_renderer.get());

    SDL_RenderPresent(m_renderer.get());
}

void Game::fetch_quit()
{
    m_isQuit = m_currentScene->get_is_quit();
}

void Game::fetch_next_scene()
{
    m_nextSceneType = m_currentScene->get_next_scene();
}

void Game::change_scene()
{
    switch (m_nextSceneType)
    {
    case SceneType::NO_CHANGE:
        break;
    case SceneType::SELECT_PLAYER:
        m_currentScene = std::make_unique<SelectPlayer>(m_renderer.get());
        break;
    case SceneType::PLAY_PVE:
        m_currentScene = std::make_unique<PlayScene>(m_renderer.get(), false);
        break;
    case SceneType::PLAY_PVP:
        m_currentScene = std::make_unique<PlayScene>(m_renderer.get(), true);
        break;
    
    default:
        throw std::logic_error("Tried to change to SceneType::" + std::to_string(m_nextSceneType) + " which is not implemented");
    }
}

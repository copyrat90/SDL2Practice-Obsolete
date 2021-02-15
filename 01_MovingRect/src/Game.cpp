#include "Game.hpp"


Game::Game(std::string title, Uint32 fps, int x, int y, int w, int h, Uint32 windowFlags)
    : m_window(nullptr, sosim::SDL_Deleter()), m_renderer(nullptr, sosim::SDL_Deleter())
{
    m_ready = false;
    m_running = false;

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

    m_running = true;

    // Manage two seperate deltaTicks for delaying & updating
    Uint32 loopDeltaTicks = 0;
    Uint32 loopStartTick;
    Uint32 updateDeltaTicks = 0;
    Uint32 updateStartTick = SDL_GetTicks();

    while (m_running)
    {
        loopStartTick = SDL_GetTicks();

        handle_events();

        updateDeltaTicks = SDL_GetTicks() - updateStartTick;
        updateStartTick = SDL_GetTicks();

        update(updateDeltaTicks);

        render();

        const Uint32 TICKS_PER_FRAME = 1000 / m_fps;
        loopDeltaTicks = SDL_GetTicks() - loopStartTick;

        if (loopDeltaTicks < TICKS_PER_FRAME)
        {
            const Uint32 TICKS_TO_DELAY = TICKS_PER_FRAME - loopDeltaTicks;
            SDL_Delay(TICKS_TO_DELAY);
        }
    }
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

    m_ready = true;
    return true;
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

void Game::handle_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_EventType::SDL_QUIT:
            m_running = false;
            break;
        case SDL_EventType::SDL_KEYDOWN:
            if (!e.key.repeat)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					m_playerVelocity.y -= PLAYER_SPEED;
					break;
				case SDLK_DOWN:
					m_playerVelocity.y += PLAYER_SPEED;
					break;
				case SDLK_LEFT:
					m_playerVelocity.x -= PLAYER_SPEED;
					break;
				case SDLK_RIGHT:
					m_playerVelocity.x += PLAYER_SPEED;
					break;
				}
			}
			break;
		case SDL_EventType::SDL_KEYUP:
            if (!e.key.repeat)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					m_playerVelocity.y += PLAYER_SPEED;
					break;
				case SDLK_DOWN:
					m_playerVelocity.y -= PLAYER_SPEED;
					break;
				case SDLK_LEFT:
					m_playerVelocity.x += PLAYER_SPEED;
					break;
				case SDLK_RIGHT:
					m_playerVelocity.x -= PLAYER_SPEED;
					break;
				}
			}
			break;
        }
    }
}

void Game::update(Uint32 deltaTicks)
{
	m_playerPosition.x += m_playerVelocity.x;
	m_playerPosition.y += m_playerVelocity.y;
}

void Game::render()
{
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());
	
	SDL_SetRenderDrawColor(m_renderer.get(), 255, 255, 255, 255);
	SDL_RenderFillRect(m_renderer.get(), &m_playerPosition);

    SDL_RenderPresent(m_renderer.get());
}

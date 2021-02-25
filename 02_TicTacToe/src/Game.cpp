#include "Game.hpp"

#include <SDL_ttf.h>
#include <exception>

#define TEST
#ifdef TEST
#include <iostream>
#endif


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

bool Game::setup()
{
    m_ready = init() && load_media();
    return m_ready;
}

void Game::clean()
{
    m_renderer.reset();
    m_window.reset();
    m_font.reset();
    m_ready = false;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* Game::get_symbolO() const
{
    return m_symbolO.get();
}

SDL_Texture* Game::get_symbolX() const
{
    return m_symbolX.get();
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
    m_font = sosim::make_font("asset/font/D2Coding.ttf", 200);
    m_symbolO = sosim::make_texture_from_text(m_renderer.get(), m_font.get(), u"O", {0, 0, 255, 255});
    m_symbolX = sosim::make_texture_from_text(m_renderer.get(), m_font.get(), u"X", {255, 0, 0, 255});

    m_board = std::make_unique<Board>(SDL_Rect{155, 75, 320, 320}, std::bind(&Game::on_piece_changed, this, std::placeholders::_1, std::placeholders::_2), m_symbolO.get(), m_symbolX.get());

    return true;
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
        case SDL_EventType::SDL_MOUSEMOTION:
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
        case SDL_EventType::SDL_MOUSEBUTTONUP:
            if (m_match_ongoing)
                m_board->handle_event(e);
            break;
        }
    }
}

void Game::update(Uint32 deltaTicks)
{
    if (m_match_ongoing)
        m_board->update(deltaTicks);
}

void Game::render()
{
    SDL_SetRenderDrawColor(m_renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(m_renderer.get());

    m_board->render(m_renderer.get());

    SDL_RenderPresent(m_renderer.get());
}

// TODO : Add piece update logic
//        (Win condition check, etc...) 
void Game::on_piece_changed(int y, int x)
{
#ifdef TEST
    std::cout << "Game::on_piece_changed(" << y << ", " << x <<")\n";
#endif
    if (check_win(y,x) == Winner::PLAYER)
    {
#ifdef TEST
        std::cout << "Player Wins!\n";
#endif
        m_match_ongoing = false;
    }
    else if (check_board_full())
    {
#ifdef TEST
        std::cout << "Tie!\n";
#endif
        m_match_ongoing = false;
    }

    play_random_cpu_move();

    if (check_win(y,x) == Winner::CPU)
    {
#ifdef TEST
        std::cout << "CPU Wins!\n";
#endif
        m_match_ongoing = false;
    }
    else if (check_board_full())
    {
#ifdef TEST
        std::cout << "Tie!\n";
#endif
        m_match_ongoing = false;
    }
}

Game::Winner Game::check_win(int y, int x)
{
    Cell::Piece lastPutPiece = (*m_board)(y,x).get_piece();

    if (lastPutPiece == Cell::Piece::EMPTY)
    {
        throw std::invalid_argument(
            "last put piece is empty\n" 
            "Game::check_win("+std::to_string(y)+","+std::to_string(x)+") == Cell::Piece::EMPTY"
        );
    }

    // Vertical win check
    if ((*m_board)(0,x).get_piece() == (*m_board)(1,x).get_piece()
    && (*m_board)(1,x).get_piece() == (*m_board)(2,x).get_piece()
    && (*m_board)(2,x).get_piece() == lastPutPiece)
        return (lastPutPiece == Cell::Piece::O) ? Winner::PLAYER : Winner::CPU;
    
    // Horizontal win check
    if ((*m_board)(y,0).get_piece() == (*m_board)(y,1).get_piece()
    && (*m_board)(y,1).get_piece() == (*m_board)(y,2).get_piece()
    && (*m_board)(y,2).get_piece() == lastPutPiece)
        return (lastPutPiece == Cell::Piece::O) ? Winner::PLAYER : Winner::CPU;

    // Increasing diagonal win check
    if ((*m_board)(2,0).get_piece() == (*m_board)(1,1).get_piece()
    && (*m_board)(1,1).get_piece() == (*m_board)(0,2).get_piece()
    && (*m_board)(0,2).get_piece() == lastPutPiece)
        return (lastPutPiece == Cell::Piece::O) ? Winner::PLAYER : Winner::CPU;

    // Decreasing diagonal win check
    if ((*m_board)(0,0).get_piece() == (*m_board)(1,1).get_piece()
    && (*m_board)(1,1).get_piece() == (*m_board)(2,2).get_piece()
    && (*m_board)(2,2).get_piece() == lastPutPiece)
        return (lastPutPiece == Cell::Piece::O) ? Winner::PLAYER : Winner::CPU;
    
    return Winner::NONE;
}

bool Game::check_board_full()
{
    for (int y = 0; y < 3; ++y)
        for (int x = 0; x < 3; ++x)
            if ((*m_board)(y,x) == Cell::Piece::EMPTY)
                return false;

    return true;
}

void Game::play_random_cpu_move()
{
    // TODO
}

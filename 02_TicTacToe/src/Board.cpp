#include "Board.hpp"


Board::Board(SDL_Rect transform)
    : m_board
    {
        {
            Cell({155,75,100,100}),
            Cell({265,75,100,100}),
            Cell({375,75,100,100})
        },
        {
            Cell({155,185,100,100}),
            Cell({265,185,100,100}),
            Cell({375,185,100,100})
        },
        {
            Cell({155,295,100,100}),
            Cell({265,295,100,100}),
            Cell({375,295,100,100})
        }
    }
{
    m_transform = transform;
}

Cell& Board::get_cell(int x, int y)
{
    return m_board[y][x];
}

const SDL_Rect& Board::get_transform() const
{
    return m_transform;
}

void Board::handle_event(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_EventType::SDL_MOUSEMOTION:
    case SDL_EventType::SDL_MOUSEBUTTONDOWN:
    case SDL_EventType::SDL_MOUSEBUTTONUP:
        for (auto& row : m_board)
        {
            for (auto& cell : row)
            {
                cell.handle_event(e);
            }
        }
        break;
    }
}

void Board::render(SDL_Renderer* renderer) const
{
    for (const Cell (&row) [3] : m_board)
    {
        for (const Cell& cell : row)
        {
            cell.render(renderer);
        }
    }
}

#include "Board.hpp"


Board::Board(SDL_Rect transform, Cell::PieceChangedCallback onPieceChanged, SDL_Texture* symbolO, SDL_Texture* symbolX)
    : m_board
    {
        {
            Cell({155,75,100,100}, 0, 0, onPieceChanged, symbolO, symbolX),
            Cell({265,75,100,100}, 0, 1, onPieceChanged, symbolO, symbolX),
            Cell({375,75,100,100}, 0, 2, onPieceChanged, symbolO, symbolX)
        },
        {
            Cell({155,185,100,100}, 1, 0, onPieceChanged, symbolO, symbolX),
            Cell({265,185,100,100}, 1, 1, onPieceChanged, symbolO, symbolX),
            Cell({375,185,100,100}, 1, 2, onPieceChanged, symbolO, symbolX)
        },
        {
            Cell({155,295,100,100}, 2, 0, onPieceChanged, symbolO, symbolX),
            Cell({265,295,100,100}, 2, 1, onPieceChanged, symbolO, symbolX),
            Cell({375,295,100,100}, 2, 2, onPieceChanged, symbolO, symbolX)
        }
    }
{
    m_transform = transform;
}

Cell& Board::get_cell(int y, int x)
{
    return m_board[y][x];
}

Cell& Board::operator() (int y, int x)
{
    return get_cell(y, x);
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

void Board::update(Uint32 deltaTicks)
{
    for (auto& row : m_board)
    {
        for (auto& cell : row)
        {
            cell.update(deltaTicks);
        }
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

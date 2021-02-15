#include "Cell.hpp"


Cell::Cell(SDL_Rect transform)
{
    m_transform = transform;
    m_piece = Piece::EMPTY;

    m_mouseState = MouseState::IDLE;
    m_clicked = false;
}

Cell::Piece Cell::get_piece() const
{
    return m_piece;
}

void Cell::set_piece(Piece piece)
{
    m_piece = piece;
}

SDL_Rect Cell::get_transform() const
{
    return m_transform;
}

void Cell::handle_event(const SDL_Event& e)
{
    SDL_Point mousePos;
    switch (e.type)
    {
    case SDL_EventType::SDL_MOUSEMOTION:
        mousePos = {e.motion.x, e.motion.y};

        switch (m_mouseState)
        {
        case MouseState::IDLE:
        case MouseState::OVER:
            m_mouseState = SDL_PointInRect(&mousePos, &m_transform) ? OVER : IDLE;
            break;
        case MouseState::PRESSED_OVER:
        case MouseState::PRESSED_OUT:
            m_mouseState = SDL_PointInRect(&mousePos, &m_transform) ? PRESSED_OVER : PRESSED_OUT;
            break;
        }
        break;

    case SDL_EventType::SDL_MOUSEBUTTONDOWN:
        mousePos = {e.button.x, e.button.y};
        if (SDL_PointInRect(&mousePos, &m_transform))
            m_mouseState = MouseState::PRESSED_OVER;
        break;

    case SDL_EventType::SDL_MOUSEBUTTONUP:
        mousePos = {e.button.x, e.button.y};
        if ((m_mouseState == PRESSED_OVER || m_mouseState == PRESSED_OUT)
            && SDL_PointInRect(&mousePos, &m_transform))
        {
            m_clicked = true;
            m_mouseState = MouseState::OVER;
        }
        else
        {
            m_mouseState = SDL_PointInRect(&mousePos, &m_transform) ? OVER : IDLE;
        }
        break;
    }
}

void Cell::render(SDL_Renderer* renderer) const
{
    SDL_Color cellColor = CELL_COLORS[(int)m_mouseState];
    SDL_SetRenderDrawColor(renderer, cellColor.r, cellColor.g, cellColor.b, cellColor.a);
    SDL_RenderFillRect(renderer, &m_transform);

    if (m_piece == Piece::O)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        // TODO
        // Draw O symbol
    }
    else if (m_piece == Piece::X)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // TODO
        // Draw X symbol
    }
}

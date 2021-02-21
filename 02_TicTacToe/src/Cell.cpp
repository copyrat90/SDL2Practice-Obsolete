#include "Cell.hpp"


Cell::Cell(SDL_Rect transform, int y, int x, PieceChangedCallback onPieceChanged, SDL_Texture* symbolO, SDL_Texture* symbolX)
{
    c_onPieceChanged = std::move(onPieceChanged);

    m_transform = transform;
    m_piece = Piece::EMPTY;
    m_y = y;
    m_x = x;

    m_symbolO = symbolO;
    m_symbolX = symbolX;

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
    switch (e.type)
    {
    case SDL_EventType::SDL_MOUSEMOTION:
        handle_mouse_motion(e.motion);
        break;

    case SDL_EventType::SDL_MOUSEBUTTONDOWN:
        handle_mouse_button_down(e.button);
        break;

    case SDL_EventType::SDL_MOUSEBUTTONUP:
        handle_mouse_button_up(e.button);
        break;
    }
}

void Cell::update(Uint32 deltaTicks)
{
    if (m_clicked && (m_piece == Piece::EMPTY))
    {
        m_piece = Piece::O;
        c_onPieceChanged(m_y, m_x);
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
        SDL_RenderCopy(renderer, m_symbolO, nullptr, &m_transform);
    }
    else if (m_piece == Piece::X)
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderCopy(renderer, m_symbolX, nullptr, &m_transform);
    }
}

void Cell::handle_mouse_motion(const SDL_MouseMotionEvent& motion)
{
    SDL_Point mousePos = {motion.x, motion.y};

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
}

void Cell::handle_mouse_button_down(const SDL_MouseButtonEvent& button)
{
    SDL_Point mousePos = {button.x, button.y};
    if (SDL_PointInRect(&mousePos, &m_transform))
        m_mouseState = MouseState::PRESSED_OVER;
}

void Cell::handle_mouse_button_up(const SDL_MouseButtonEvent& button)
{
    SDL_Point mousePos = {button.x, button.y};
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
}
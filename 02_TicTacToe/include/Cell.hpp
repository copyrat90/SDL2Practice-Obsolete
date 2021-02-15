#ifndef CELL_HPP
#define CELL_HPP

#include <SDL.h>


class Cell
{
public:
    enum Piece { EMPTY, O, X };

    Cell(SDL_Rect transform);

    Piece get_piece() const;
    void set_piece(Piece piece);
    SDL_Rect get_transform() const;

    void handle_event(const SDL_Event& e);
    void render(SDL_Renderer* renderer) const;

private:
    Piece m_piece;
    SDL_Rect m_transform;

    enum MouseState {IDLE, OVER, PRESSED_OVER, PRESSED_OUT};
    MouseState m_mouseState;
    bool m_clicked;

    static constexpr SDL_Color CELL_COLORS[4]
    {
        {255, 255, 255, 255},
        {200, 200, 200, 255},
        {100, 100, 100, 255},
        {128, 128, 128, 255}
    };
};


#endif

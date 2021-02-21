#ifndef CELL_HPP
#define CELL_HPP

#include <SDL.h>
#include <functional>


class Cell
{
public:
    using PieceChangedCallback = std::function<void(int,int)>;
    enum Piece { EMPTY, O, X };

    Cell(SDL_Rect transform, int y, int x, PieceChangedCallback onPieceChanged, SDL_Texture*, SDL_Texture*);

    Piece get_piece() const;
    void set_piece(Piece piece);
    SDL_Rect get_transform() const;

    void handle_event(const SDL_Event&);
    void update(Uint32 deltaTicks);
    void render(SDL_Renderer* renderer) const;

private:
    Piece m_piece;
    SDL_Rect m_transform;

    int m_y, m_x;

    enum MouseState {IDLE, OVER, PRESSED_OVER, PRESSED_OUT};
    MouseState m_mouseState;
    bool m_clicked;

    PieceChangedCallback c_onPieceChanged;
    SDL_Texture* m_symbolO;
    SDL_Texture* m_symbolX;

    static constexpr SDL_Color CELL_COLORS[4]
    {
        {255, 255, 255, 255},
        {200, 200, 200, 255},
        {100, 100, 100, 255},
        {128, 128, 128, 255}
    };

    void handle_mouse_motion(const SDL_MouseMotionEvent&);
    void handle_mouse_button_down(const SDL_MouseButtonEvent&);
    void handle_mouse_button_up(const SDL_MouseButtonEvent&);
};


#endif

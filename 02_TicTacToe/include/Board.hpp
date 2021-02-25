#ifndef BOARD_HPP
#define BOARD_HPP

#include "Cell.hpp"


class Board
{
public:
    Board(SDL_Rect transform, Cell::PieceChangedCallback, SDL_Texture*, SDL_Texture*);

    Cell& get_cell(int y, int x);
    Cell& operator() (int y, int x);
    const SDL_Rect& get_transform() const;

    void handle_event(const SDL_Event& e);
    void update(Uint32 deltaTicks);
    void render(SDL_Renderer* renderer) const;

private:
    Cell m_board[3][3];
    SDL_Rect m_transform;
};


#endif

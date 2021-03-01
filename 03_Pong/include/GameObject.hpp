#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include <SDL.h>
#include <chrono>

namespace chrono = std::chrono;


class GameObject
{
public:
    virtual void handle_event(const SDL_Event&) = 0;
    virtual void update(const chrono::milliseconds&) = 0;
    virtual void render(SDL_Renderer*) const = 0;

    virtual ~GameObject() {}
};


#endif
#ifndef SDL_EXCEPTION_HPP
#define SDL_EXCEPTION_HPP

#include <exception>
#include <SDL.h>

class SDL_Exception : public std::exception
{
public:
    const char* what() const noexcept override
    {
        return SDL_GetError();
    }
};


#endif
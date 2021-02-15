#include "sosim/SDL_Factory.hpp"

namespace sosim
{
    u_ptr<SDL_Window> make_window(const char *title, int x, int y, int w, int h, Uint32 flags)
    {
        return u_ptr<SDL_Window>(SDL_CreateWindow(title, x, y, w, h, flags), SDL_Deleter());
    }

    u_ptr<SDL_Renderer> make_renderer(SDL_Window *window, int index, Uint32 flags)
    {
        return u_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, flags), SDL_Deleter());
    }

    u_ptr<SDL_Surface> make_surface(std::string path)
    {
        return u_ptr<SDL_Surface> (IMG_Load(path.c_str()), SDL_Deleter());
    }

    u_ptr<SDL_Texture> make_texture(SDL_Renderer* renderer, std::string path)
    {
        auto surface = make_surface(path);
        return u_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface.get()), SDL_Deleter());
    }
}
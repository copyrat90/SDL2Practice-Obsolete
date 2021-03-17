#include "sosim/SDL_Factory.hpp"
#include "sosim/SDL_Exception.hpp"


namespace sosim
{
    u_ptr<SDL_Window> make_window(const char *title, int x, int y, int w, int h, Uint32 flags)
    {
        auto window = u_ptr<SDL_Window>(SDL_CreateWindow(title, x, y, w, h, flags), SDL_Deleter());
        if (!window)
            throw SDL_Exception();
        return window;
    }

    u_ptr<SDL_Renderer> make_renderer(SDL_Window *window, int index, Uint32 flags)
    {
        auto renderer = u_ptr<SDL_Renderer>(SDL_CreateRenderer(window, index, flags), SDL_Deleter());
        if (!renderer)
            throw SDL_Exception();
        return renderer;
    }

    u_ptr<SDL_Surface> make_surface(std::string path)
    {
        auto surface = u_ptr<SDL_Surface> (IMG_Load(path.c_str()), SDL_Deleter());
        if (!surface)
            throw SDL_Exception();
        return surface;
    }

    u_ptr<SDL_Texture> make_texture(SDL_Renderer* renderer, std::string path)
    {
        auto surface = make_surface(path);
        return make_texture_from_surface(renderer, surface.get());
    }

    u_ptr<SDL_Texture> make_texture_from_surface(SDL_Renderer* renderer, SDL_Surface* surface)
    {
        auto texture = u_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, surface), SDL_Deleter());
        if (!texture)
            throw SDL_Exception();
        return texture;
    }

    u_ptr<SDL_Texture> make_texture_from_text(SDL_Renderer* renderer, TTF_Font* font, const std::u16string& text, SDL_Color fg)
    {
        SDL_Surface* surface = TTF_RenderUNICODE_Solid(font, (const Uint16*)(text.c_str()), fg);
        auto texture = make_texture_from_surface(renderer, surface);

        SDL_FreeSurface(surface);
        return texture;
    }

    u_ptr<SDL_Texture> make_texture_from_text(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color fg)
    {
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), fg);
        auto texture = make_texture_from_surface(renderer, surface);

        SDL_FreeSurface(surface);
        return texture;
    }

    u_ptr<TTF_Font> make_font(std::string path, int size)
    {
        auto font = u_ptr<TTF_Font>(TTF_OpenFont(path.c_str(), size), SDL_Deleter());
        if (!font)
            throw SDL_Exception();
        return font;
    }
}
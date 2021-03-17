#ifndef SDL_FACTORY_HPP
#define SDL_FACTORY_HPP

#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

namespace sosim
{
    struct SDL_Deleter
    {
        void operator()(SDL_Window* p) const { SDL_DestroyWindow(p); }
        void operator()(SDL_Renderer* p) const { SDL_DestroyRenderer(p); }
        void operator()(SDL_Surface* p) const { SDL_FreeSurface(p); }
        void operator()(SDL_Texture* p) const { SDL_DestroyTexture(p); }
        void operator()(TTF_Font* p) const { TTF_CloseFont(p); }
    };

    template <typename T>
    using u_ptr = std::unique_ptr<T, SDL_Deleter>;

    u_ptr<SDL_Window> make_window(const char *title, int x, int y, int w, int h, Uint32 flags);
    u_ptr<SDL_Renderer> make_renderer(SDL_Window *window, int index, Uint32 flags);
    u_ptr<SDL_Surface> make_surface(std::string path);
    u_ptr<SDL_Texture> make_texture(SDL_Renderer* renderer, std::string path);
    u_ptr<SDL_Texture> make_texture_from_surface(SDL_Renderer* renderer, SDL_Surface* surface);
    u_ptr<SDL_Texture> make_texture_from_text(SDL_Renderer* renderer, TTF_Font* font, const std::u16string& text, SDL_Color fg);
    u_ptr<SDL_Texture> make_texture_from_text(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color fg);
    u_ptr<TTF_Font> make_font(std::string path, int size);
}


#endif
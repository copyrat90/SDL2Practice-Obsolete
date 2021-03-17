#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SDL.h>
#include <functional>
#include "sosim/SDL_Factory.hpp"
#include "GameObject.hpp"


namespace scene::ui
{
    class Button : public GameObject
    {
    public:
        Button(const std::u16string&, TTF_Font*, SDL_Renderer*, SDL_Rect transform);

        void handle_event(const SDL_Event&) override;
        void update(const chrono::milliseconds&) override;
        void render(SDL_Renderer*) const override;

        void set_on_mouse_click(std::function<void()> onMouseClick) { m_onMouseClick = onMouseClick; }

        void set_transform(SDL_Rect transform) { m_transform = transform; }
        SDL_Rect get_transform() const { return m_transform; }

    private:
        void handle_mouse_motion(const SDL_MouseMotionEvent&);
        void handle_mouse_button_down(const SDL_MouseButtonEvent&);
        void handle_mouse_button_up(const SDL_MouseButtonEvent&);

        std::function<void()> m_onMouseClick;

        bool m_isMouseHover;
        bool m_isMouseDownOnHover;

        SDL_Rect m_transform;
        std::u16string m_text;
        sosim::u_ptr<SDL_Texture> m_textTexture;

        constexpr static SDL_Color FILL_COLOR_MOUSE_HOVER = {211,211,211,255};
        constexpr static SDL_Color FILL_COLOR_MOUSE_OUT = {0,0,0,255};
        constexpr static SDL_Color LINE_COLOR_MOUSE_HOVER = {59,59,59,255};
        constexpr static SDL_Color LINE_COLOR_MOUSE_OUT = {211,211,211,255};
    };
}


#endif
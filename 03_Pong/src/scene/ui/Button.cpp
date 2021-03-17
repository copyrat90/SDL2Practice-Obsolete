#include "scene/ui/Button.hpp"



namespace scene::ui
{
    Button::Button(const std::u16string& text, TTF_Font* font, SDL_Renderer* renderer, SDL_Rect transform)
    {
        m_text = text;
        m_textTexture = sosim::make_texture_from_text(renderer, font, text, {255,255,255,255});
        m_transform = transform;

        SDL_Point mousePos;
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
        m_isMouseHover = SDL_PointInRect(&mousePos, &m_transform);

        m_isMouseDownOnHover = false;
    }

    void Button::handle_event(const SDL_Event& e)
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

    void Button::update(const chrono::milliseconds& deltaTime)
    {

    }

    void Button::render(SDL_Renderer* renderer) const
    {
        SDL_Color fill, line;
        if (m_isMouseHover)
        {
            fill = FILL_COLOR_MOUSE_HOVER;
            line = LINE_COLOR_MOUSE_HOVER;
        }
        else
        {
            fill = FILL_COLOR_MOUSE_OUT;
            line = LINE_COLOR_MOUSE_OUT;
        }
        
        SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
        SDL_RenderFillRect(renderer, &m_transform);

        SDL_SetTextureColorMod(m_textTexture.get(), line.r, line.g, line.b);
        SDL_RenderCopy(renderer, m_textTexture.get(), nullptr, &m_transform);

        SDL_SetRenderDrawColor(renderer, line.r, line.g, line.b, line.a);
        SDL_RenderDrawRect(renderer, &m_transform);
    }

    void Button::handle_mouse_motion(const SDL_MouseMotionEvent& e)
    {
        const SDL_Point mousePos {e.x, e.y};
        m_isMouseHover = SDL_PointInRect(&mousePos, &m_transform);
    }

    void Button::handle_mouse_button_down(const SDL_MouseButtonEvent& e)
    {
        if (m_isMouseHover)
        {
            m_isMouseDownOnHover = true;
        }
    }

    void Button::handle_mouse_button_up(const SDL_MouseButtonEvent& e)
    {
        const SDL_Point mousePos {e.x, e.y};
        bool isMouseUpOnHover = SDL_PointInRect(&mousePos, &m_transform);

        if (m_isMouseDownOnHover && isMouseUpOnHover)
        {
            m_isMouseDownOnHover = false;
            m_onMouseClick();
        }
        m_isMouseDownOnHover = false;
    }
}

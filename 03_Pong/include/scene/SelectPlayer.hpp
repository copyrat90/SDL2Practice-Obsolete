#ifndef SELECT_PLAYER_HPP
#define SELECT_PLAYER_HPP

#include "Scene.hpp"
#include "scene/ui/Button.hpp"
#include <string>


namespace scene
{
    class SelectPlayer : public Scene
    {
    public:
        SelectPlayer() = delete;
        SelectPlayer(SDL_Renderer*);

        void handle_events() override;
        void update(const chrono::milliseconds&) override;
        void render(SDL_Renderer*) override;

    private:
        const std::u16string PVE_STR = u"CPU vs Player";
        const std::u16string PVP_STR = u"Player vs Player";

        ui::Button m_pve;
        ui::Button m_pvp;

        void c_on_button_click(std::u16string);
    };
}


#endif
#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL.h>
#include <chrono>
#include <functional>

#include "SceneType.hpp"

namespace chrono = std::chrono;


namespace scene
{
    class Scene
    {
    public:
        virtual void handle_events() = 0;
        virtual void update(const chrono::milliseconds&) = 0;
        virtual void render(SDL_Renderer* const) = 0;

        virtual ~Scene() {}

        SceneType get_next_scene() const { return m_nextScene; }
        bool get_is_quit() const { return m_isQuit; }
    
    protected:
        SceneType m_nextScene = SceneType::NO_CHANGE;
        bool m_isQuit = false;
    };
}


#endif
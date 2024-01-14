#pragma once

#include <Engine/Graphic/Window.hpp>
#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Utils/Timer.hpp>

#include "TrainGame/App/View.hpp"
#include "TrainGame/App/Objects/GameObject.hpp"

namespace Thomas
{
    class Game : private Event::Subscriber
    {
    public:
        Game();
        ~Game();

        Window* window() const { return m_window.get(); }

        void createScene();
    private:
        void onStateUpdate(const CommonEvents::StateUpdated& evt);
        void onKeyPressed(const CommonEvents::KeyPressed& evt);
        void onMouseMoved(const CommonEvents::MouseMoved& evt);

        Timer::Chronometre      m_timer;
        std::unique_ptr<Window> m_window;
        std::shared_ptr<View>   m_view;

        std::unordered_map<uint64_t, GameObject*> m_objects;
    };
} // namespace Thomas
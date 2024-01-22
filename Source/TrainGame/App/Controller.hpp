#pragma once

#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Utils/Timer.hpp>

namespace Thomas
{
    struct Controller : private Event::Subscriber {
        Controller(std::shared_ptr<Scene> scene);

    protected:
        // Events
        void _on_state_update(const CommonEvents::StateUpdated& evt);
        void _on_key_pressed(const CommonEvents::KeyPressed& evt);
        void _on_mouse_moved(const CommonEvents::MouseMoved& evt);

    private:
        std::shared_ptr<Scene> m_scene;
        Timer::Chronometre m_timer;
    };
} // namespace Thomas
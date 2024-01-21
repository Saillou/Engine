#pragma once

#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/BaseScene.hpp>
#include <Engine/Utils/Timer.hpp>

#include "View.hpp"

struct Controller: private Event::Subscriber {
    Controller(std::shared_ptr<BaseScene> scene);

protected:
    // Events
    void _on_state_update(const CommonEvents::StateUpdated& evt);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    void _on_mouse_moved(const CommonEvents::MouseMoved& evt);
    void _on_mouse_button(const CommonEvents::MouseButton& evt);

private:
    std::shared_ptr<View> m_view;
    std::vector<Light> m_pontential_lights;
    Timer::Chronometre m_timer;
};

#pragma once

#include <Engine/Events/CommonEvents.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Utils/Timer.hpp>

#include "View.hpp"
#include "Ui.hpp"

struct Controller: private Event::Subscriber {
    Controller(Ui& ui, View& view);

protected:
    // Events
    void _on_state_update(const CommonEvents::StateUpdated& evt);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);
    void _on_mouse_moved(const CommonEvents::MouseMoved& evt);
    void _on_mouse_button(const CommonEvents::MouseButton& evt);

    void _on_ui_update(const Ui*, const CommonEvents::StateUpdated& evt);

private:
    Ui& m_ui;
    View& m_view;
    std::vector<Light> m_pontential_lights;
    Timer::Chronometre m_timer;

    float m_distance = 6.0f;
    float m_theta = 0.0f;
};

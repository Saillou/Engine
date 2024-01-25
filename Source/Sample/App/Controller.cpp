#include "Controller.hpp"

#include <memory>
#include <iostream>

Controller::Controller(Ui& ui, View& view):
    m_ui(ui),
    m_view(view)
{
    // Root events
    _subscribe(&Controller::_on_state_update);
    _subscribe(&Controller::_on_key_pressed);
    _subscribe(&Controller::_on_mouse_moved);
    _subscribe(&Controller::_on_mouse_button);

    _subscribe(&ui, &Controller::_on_ui_update);

    // Camera
    m_view.scene().camera().position = glm::vec3(m_distance, 0, 1.25f);
    m_view.scene().camera().direction = glm::vec3(0, 0, 0);

    // Lights
    m_pontential_lights = {
        Light(glm::vec3{  0,  -1.50f, 3.0f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
        Light(glm::vec3{  0,  +1.50f, 3.0f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  0,    0,    3.0f }, glm::vec4{ 0.3, 1, 0.7, 1 }),
        Light(glm::vec3{  -1.50f,  0, 3.0f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  +1.50f,  0, 3.0f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
    };
    m_view.scene().lights() = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + 2);

    // Start
    m_timer.tic();
}

// Events
void Controller::_on_state_update(const CommonEvents::StateUpdated& evt) {
    // .. do stuff ..
}

void Controller::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    // Nothing until click on start
    if (m_ui.state() < Ui::State::InGame)
        return;

    // Only one thing to do
    if (m_ui.state() == Ui::State::Pause) {
        if (evt.action == Action::Pressed && evt.key == Key::Space)
            m_ui.setState(Ui::State::InGame);
        return;
    }

    // Camera movement
    if(evt.action == Action::Pressed || evt.action == Action::Repeated) 
    {
        glm::vec3 dir(0, 0, 0);

        switch (evt.key)
        {
            case Key::ArrowRight: dir.x = +1.0f; break;
            case Key::ArrowLeft:  dir.x = -1.0f; break;
            case Key::ArrowUp:    dir.y = -1.0f; break;
            case Key::ArrowDown:  dir.y = +1.0f; break;
            case 'Q':             dir.z = +1.0f; break;
            case 'W':             dir.z = -1.0f; break;
        }

        if (dir != glm::vec3(0, 0, 0)) 
        {
            m_theta    += 0.01f * dir.x;
            m_distance += 0.05f * dir.y;

            m_view.scene().camera().position.x = m_distance * cos(m_theta);
            m_view.scene().camera().position.y = m_distance * sin(m_theta);
            m_view.scene().camera().position.z += 0.05f * dir.z;
        }
    }

    // Actions
    if (evt.action == Action::Pressed)
    {
        switch (evt.key)
        {
            case 'R': m_view.enable_filter      = !m_view.enable_filter;      break;
            case 'T': m_view.enable_interaction = !m_view.enable_interaction; break;

            case Key::Space: m_ui.setState(Ui::State::Pause); break;
        }
    }
    

    // Lights
    if (evt.action == Action::Pressed) 
    {
        size_t nLightsEnabled = -1;
        switch (evt.key)
        {
            case Numpad_0 + 0: nLightsEnabled = 0; break;
            case Numpad_0 + 1: nLightsEnabled = 1; break;
            case Numpad_0 + 2: nLightsEnabled = 2; break;
            case Numpad_0 + 3: nLightsEnabled = 3; break;
            case Numpad_0 + 4: nLightsEnabled = 4; break;
            case Numpad_0 + 5: nLightsEnabled = 5; break;
        }

        if (nLightsEnabled != -1) {
            m_view.scene().lights() = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + nLightsEnabled);
        }
    }
}

void Controller::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // ..
}

void Controller::_on_mouse_button(const CommonEvents::MouseButton& evt) {
    // ..
}

void Controller::_on_ui_update(const CommonEvents::StateUpdated& evt) {
    if (m_ui.state() == Ui::State::InGame) {
        m_view.enable_interaction = true;
    }
}

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
    m_view.scene().camera().position  = glm::vec3(m_distance, 0, 1.25f);
    m_view.scene().camera().direction = glm::vec3(0, 0, 0);

    // Lights
    m_pontential_lights = {
        Light(glm::vec3{ 0,  -1.50f, 3.0f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
        Light(glm::vec3{ 0,  +1.50f, 3.0f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{ 0,    0,    3.0f }, glm::vec4{ 0.3, 1, 0.7, 1 }),
        Light(glm::vec3{ -1.50f,  0, 3.0f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{ +1.50f,  0, 3.0f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
    };
    m_view.scene().lights() = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + m_ui.getLightsCount());

    // States
    m_view.enable_filter      = m_enable_filter;
    m_view.enable_interaction = m_enable_interaction;
}

// Events
void Controller::_on_state_update(const CommonEvents::StateUpdated& evt) {
    // .. do stuff ..
}

void Controller::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    // Nothing until click on start
    if (m_ui.state() < Ui::State::InGame)
        return;

    // Pause
    if (m_ui.state() == Ui::State::Pause) 
    {
        if (evt.action == InputAction::Pressed && evt.key == KeyCode::Escape) {
            m_ui.setState(Ui::State::InGame);
            m_view.enable_interaction = m_enable_interaction;
        }

        return;
    }

    // In game
    if(evt.action == InputAction::Pressed || evt.action == InputAction::Repeated)
    {
        glm::vec3 dir(0, 0, 0);

        switch (evt.key)
        {
            case KeyCode::ArrowRight: dir.x = +1.0f; break;
            case KeyCode::ArrowLeft:  dir.x = -1.0f; break;
            case KeyCode::ArrowUp:    dir.y = -1.0f; break;
            case KeyCode::ArrowDown:  dir.y = +1.0f; break;
            case 'Q':                 dir.z = +1.0f; break;
            case 'W':                 dir.z = -1.0f; break;
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
    if (evt.action == InputAction::Pressed)
    {
        switch (evt.key)
        {
            case 'R': m_view.enable_filter      = (m_enable_filter       = !m_enable_filter);         break;
            case 'T': m_view.enable_interaction = (m_enable_interaction  = !m_enable_interaction);    break;

            case KeyCode::Escape: m_ui.setState(Ui::State::Pause); break;
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
    switch (m_ui.state()) 
    {
        case Ui::State::InGame:
            m_view.scene().lights() = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + m_ui.getLightsCount());
            m_view.enable_interaction = m_enable_interaction;
            break;

        case Ui::State::Pause:
            m_view.enable_interaction = false;
            break;
    }
}

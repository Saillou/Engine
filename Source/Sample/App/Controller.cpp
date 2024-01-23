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
    m_view.scene().lights() = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + 5);

    // Start
    m_timer.tic();
}

// Events
void Controller::_on_state_update(const CommonEvents::StateUpdated& evt) {
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f;

    // .. do stuff ..

    m_timer.tic();
}

void Controller::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    // Movement
    glm::vec3 dir(0, 0, 0);

    if(evt.action == Action::Pressed || evt.action == Action::Repeated) 
    {
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
            case 'R': m_view.enable_filter = !m_view.enable_filter; break;
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
    std::string msg_mouse = "Mouse button: ";

    switch (evt.button) 
    {
        case Button::Left:  msg_mouse += "left" ; break;
        case Button::Right: msg_mouse += "right"; break;
    }

    msg_mouse += " is ";

    switch (evt.action)
    {
        case Action::Pressed:  msg_mouse += "pressed";  break;
        case Action::Released: msg_mouse += "released"; break;
        case Action::Repeated: msg_mouse += "repeated"; break;
    }

    std::cout << msg_mouse << std::endl;
}

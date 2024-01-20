#include "Controller.hpp"

#include <memory>
#include <iostream>

Controller::Controller(std::shared_ptr<BaseScene> scene):
    m_view(std::dynamic_pointer_cast<View>(scene))
{
    assert(m_view != nullptr);

    // Root events
    _subscribe(&Controller::_on_state_update);
    _subscribe(&Controller::_on_key_pressed);
    _subscribe(&Controller::_on_mouse_moved);
    _subscribe(&Controller::_on_mouse_clicked);

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
    // Mouvement
    glm::vec3 dir(0, 0, 0);

    switch (evt.key)
    {
        case Key::ArrowRight: dir.x = +1.0f; break;
        case Key::ArrowLeft:  dir.x = -1.0f; break;
        case Key::ArrowUp:    dir.y = +1.0f; break;
        case Key::ArrowDown:  dir.y = -1.0f; break;
        case 'Q':             dir.z = +1.0f; break;
        case 'W':             dir.z = -1.0f; break;
    }

    if (dir != glm::vec3(0, 0, 0)) 
    {
        m_view->camera().position += 0.05f * dir;
    }

    // Lights
    const std::vector<Light> pontential_lights = {
        Light(glm::vec3{  0,  -1.50f, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
        Light(glm::vec3{  0,  +1.50f, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  0,    0,    0.7f }, glm::vec4{ 0.3, 1, 0.7, 1 }),
        Light(glm::vec3{  -1.50f,  0, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  +1.50f,  0, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
    };
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
        m_view->lights() = std::vector<Light>(pontential_lights.cbegin(), pontential_lights.cbegin() + nLightsEnabled);
    }

    //// Other
    //switch (evt.key)
    //{
    //    case 'R': m_view->enable_filter = true;  break;
    //    case 'T': m_view->enable_filter = false; break;
    //}
}

void Controller::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    m_view->mouse_on(evt.x, evt.y);
}

void Controller::_on_mouse_clicked(const CommonEvents::MouseClicked& evt) {
    switch (evt.button) 
    {
        case Button::Left:  std::cout << "Click left"  << std::endl; break;
        case Button::Right: std::cout << "Click right" << std::endl; break;
    }
}

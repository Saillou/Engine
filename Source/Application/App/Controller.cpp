#include "Controller.hpp"
#include <iostream>

Controller::Controller(std::shared_ptr<BaseScene> scene):
    m_scene(std::dynamic_pointer_cast<View>(scene))
{
    // Root events
    _subscribe(&Controller::_on_state_update);
    _subscribe(&Controller::_on_key_pressed);
    _subscribe(&Controller::_on_mouse_moved);

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
    glm::vec2 dir(0, 0);

    switch (evt.key)
    {
        case Key::ArrowRight: dir.x = +1.0f; break;
        case Key::ArrowLeft:  dir.x = -1.0f; break;
        case Key::ArrowUp:    dir.y = +1.0f; break;
        case Key::ArrowDown:  dir.y = -1.0f; break;
    }

    if (dir != glm::vec2(0, 0)) 
    {
        m_scene->camera().position += glm::vec3(0.1f*dir, 0);
    }
}

void Controller::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // ..
}

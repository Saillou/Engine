#include "Controller.hpp"
#include <iostream>

// Instance
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
    if(evt.key == Key::Space)
        m_scene->redraw = true;
}

void Controller::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // ..
}

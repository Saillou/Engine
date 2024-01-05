#include "Controller.hpp"
#include <iostream>

#include "View.hpp"

Controller::Controller(std::shared_ptr<BaseScene> scene):
    m_scene(scene)
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

    // physic of ball here: m_scene->ballPos(new pos);

    m_timer.tic();
}

void Controller::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    glm::vec2 dir(0, 0);

    switch (evt.key)
    {
        case Key::ArrowRight: dir.x = +0.05f; break;
        case Key::ArrowLeft:  dir.x = -0.05f; break;
        case Key::ArrowUp:    dir.y = +0.05f; break;
        case Key::ArrowDown:  dir.y = -0.05f; break;
    }

    auto m_view = std::dynamic_pointer_cast<View>(m_scene);
    //m_view->movePlayer2Pos(glm::vec3(dir.x,0, dir.y));
}

void Controller::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // ..
}

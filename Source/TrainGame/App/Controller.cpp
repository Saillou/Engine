#include "Controller.hpp"
#include "View.hpp"

#include <memory>
#include <iostream>

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
        m_scene->camera().position += 0.05f * dir;
        m_scene->lights()[0].position += 0.05f * dir;
    }

    // Other
    auto view = std::dynamic_pointer_cast<View>(m_scene);

    switch (evt.key)
    {
        case 'R': view->enable_filter = true; break;
        case 'T': view->enable_filter = false; break;
    }
}

void Controller::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    // ..
}

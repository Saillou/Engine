#include "SampleForest.hpp"

#include <iostream>

SampleForest::SampleForest() :
    m_scene(Service<Window>::get().scene()),
    m_view(std::make_unique<ViewForest>())
{
    // Root events
    _subscribe(&SampleForest::_on_state_update);
    _subscribe(&SampleForest::_on_key_pressed);
    _subscribe(&SampleForest::_on_mouse_moved);

    // Lights
    m_scene.lights() = { 
        Light(glm::vec3{ 0,  -1.50f, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 })
    };

    // Camera
    m_scene.camera().position  = glm::vec3(1, -4, 3);
    m_scene.camera().direction = glm::vec3(0.0f, 0.0f, 0.0f);

    // Start
    m_timer.tic();
}

bool SampleForest::wantQuit() const {
    return m_want_quit;
}

// Events
void SampleForest::_on_state_update(const CommonEvents::StateUpdated& evt) {
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f;

    // .. do stuff ..

    m_timer.tic();
}

void SampleForest::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    // Mouvement
    glm::vec3 dir(0, 0, 0);

    switch (evt.key)
    {
        case ArrowRight: dir.x = +1.0f; break;
        case ArrowLeft:  dir.x = -1.0f; break;
        case ArrowUp:    dir.y = +1.0f; break;
        case ArrowDown:  dir.y = -1.0f; break;
        case 'Q':        dir.z = +1.0f; break;
        case 'W':        dir.z = -1.0f; break;
    }

    if (dir != glm::vec3(0, 0, 0))
    {
        m_scene.camera().position += 0.05f * dir;
    }

    // Filter
    switch (evt.key)
    {
        case 'R': m_view->enable_filter = true;  break;
        case 'T': m_view->enable_filter = false; break;
    }

    // Other actions
    switch (evt.key)
    {
        case Escape: m_want_quit = true;
    }
}

void SampleForest::_on_mouse_moved(const CommonEvents::MouseMoved& evt) {
    m_view->mouse_on(evt.x, evt.y);
}

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
    _subscribe(&SampleForest::_on_mouse_clicked);

    // Lights
    m_pontential_lights = {
        Light(glm::vec3{  0,  -1.50f, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
        Light(glm::vec3{  0,  +1.50f, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  0,    0,    0.7f }, glm::vec4{ 0.3, 1, 0.7, 1 }),
        Light(glm::vec3{  -1.50f,  0, 0.7f }, glm::vec4{ 0.7, 0.3, 1, 1 }),
        Light(glm::vec3{  +1.50f,  0, 0.7f }, glm::vec4{ 1, 0.7, 0.3, 1 }),
    };
    m_scene.lights() = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + 2);

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

    // Lights
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
        m_scene.lights() = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + nLightsEnabled);
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

void SampleForest::_on_mouse_clicked(const CommonEvents::MouseButton& evt) {
    switch (evt.button)
    {
        case Left:  std::cout << "Click left" << std::endl; break;
        case Right: std::cout << "Click right" << std::endl; break;
    }
}
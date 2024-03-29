#include "SampleCube.hpp"
#include "ViewCube.hpp"
#include "UiCube.hpp"

#include <memory>
#include <iostream>

SampleCube::SampleCube()
{
    Scene& scene = Service<Window>::get().scene();
    m_ui   = std::make_unique<UiCube>();
    m_view = std::make_unique<ViewCube>();

    // Root events
    _subscribe(&SampleCube::_on_state_update);
    _subscribe(&SampleCube::_on_key_pressed);
    _subscribe(m_ui.get(), &SampleCube::_on_ui_update);

    // Camera
    scene.camera.position  = glm::vec3(m_distance, 0, 1.25f);
    scene.camera.direction = glm::vec3(0, 0, 0);

    // Lights
    m_pontential_lights = {
        Light(glm::vec3{ 0,   +3.0f, 3.0f }, glm::vec4{ 1, 0.5f, 1, 1 }),
        Light(glm::vec3{ 0,   -3.0f, 3.0f }, glm::vec4{ 1, 1, 0.5f, 1 }),
        Light(glm::vec3{ 0,      0,  3.0f }, glm::vec4{ 0.5f, 0, 1, 1 }),
        Light(glm::vec3{ -3.0f,  0,  3.0f }, glm::vec4{ 1, 1, 0.5f, 1 }),
        Light(glm::vec3{ +3.0f,  0,  3.0f }, glm::vec4{ 1, 0.5f, 1, 1 }),
    };
    scene.lights = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + m_ui->lightsCount());
}

// Events
void SampleCube::_on_state_update(const CommonEvents::StateUpdated& evt) {
    wantQuit = m_ui->wantQuit();
}

void SampleCube::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    Scene& scene = Service<Window>::get().scene();

    // Nothing until click on start
    if (m_ui->state() < UiCube::State::InGame)
        return;

    // Pause
    if (m_ui->state() == UiCube::State::Pause)
    {
        if (evt.action == InputAction::Pressed && evt.key == KeyCode::Escape) {
            m_ui->setState(UiCube::State::InGame);
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

            scene.camera.position.x = m_distance * cos(m_theta);
            scene.camera.position.y = m_distance * sin(m_theta);
            scene.camera.position.z += 0.05f * dir.z;
        }
    }

    // Actions
    if (evt.action == InputAction::Pressed)
    {
        switch (evt.key)
        {
            case KeyCode::Escape: m_ui->setState(UiCube::State::Pause); break;
        }
    }
}

void SampleCube::_on_ui_update(const CommonEvents::StateUpdated& evt) {
    Scene& scene = Service<Window>::get().scene();

    switch (m_ui->state()) 
    {
        case UiCube::State::InGame:
            scene.lights = std::vector<Light>(m_pontential_lights.cbegin(), m_pontential_lights.cbegin() + m_ui->lightsCount());
            break;
    }
}

#include "SamplePong.hpp"

// Particles
SamplePong::SamplePong() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = { {glm::vec3(0,0,3.0f), glm::vec4(1.0f) } };

    // Camera
    m_scene.camera().position = glm::vec3(m_distance, 0, 1.25f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Entities shape
    m_entities["cube"] = Entity(Entity::SimpleShape::Cube);
    m_entities["cube"].poses() = { glm::mat4(1.0f) };
    m_entities["cube"].localMaterial().diffuse_color = glm::vec4(1.0f, 0.2f, 0.9f, 0.9f);

    // Enable events
    _subscribe(&SamplePong::_draw);
    _subscribe(&SamplePong::_on_key_pressed);

    // Go
    m_timer.tic();
}

// Events
void SamplePong::_draw(const SceneEvents::Draw&)
{
    // Draw items
    m_scene.renderer().draw(Render::DrawType::Lights, m_entities["cube"]);

    m_timer.tic();
}

void SamplePong::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    Scene& scene = Service<Window>::get().scene();

    if (evt.action == InputAction::Pressed || evt.action == InputAction::Repeated)
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
            m_theta += 0.01f * dir.x;
            m_distance += 0.05f * dir.y;

            scene.camera().position.x = m_distance * cos(m_theta);
            scene.camera().position.y = m_distance * sin(m_theta);
            scene.camera().position.z += 0.05f * dir.z;
        }
    }
}
#include "SamplePong.hpp"

// Particles
SamplePong::SamplePong() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = { {glm::vec3(-1.0f, -2.0f, 2.50f), glm::vec4(1.0f) } };

    // Camera
    m_scene.camera().position = glm::vec3(0, m_distance, 0);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Create entities shape
    m_entities["field"] = Entity(Entity::SimpleShape::Quad);
    m_entities["field"].localMaterial().diffuse_color = glm::vec4(1,1,1,0.2f);

    glm::mat4& field_pose = m_entities["field"].localPose();
    field_pose = glm::translate(field_pose, glm::vec3(0,0.1f,0));
    field_pose = glm::scale(field_pose, glm::vec3(2.0f));
    field_pose = glm::rotate(field_pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    m_entities["player"] = Entity(Entity::SimpleShape::Cube);
    m_entities["player"].localMaterial().diffuse_color = glm::vec4(1.0f);
    m_entities["player"].localPose() = { glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.1f, 0.1f)) };

    m_entities["ball"] = Entity(Entity::SimpleShape::Sphere);
    m_entities["ball"].localMaterial().diffuse_color = glm::vec4(1.0f);
    m_entities["ball"].localPose() = { glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)) };

    // Populate entities
    m_entities["player"].poses() = { 
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)),
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, +2.0f)),
    };

    m_entities["ball"].poses() = {
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
    };

    m_entities["field"].poses() = {
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))
    };

    // Enable events
    _subscribe(&SamplePong::_draw);
    _subscribe(&SamplePong::_on_key_pressed);

    // Go
    m_timer.tic();
}

// Events
void SamplePong::_draw(const SceneEvents::Draw&)
{
    for (auto& entity : m_entities) {
        m_scene.renderer().draw(Render::DrawType::Shadows, entity.second);
    }

    m_timer.tic();
}

void SamplePong::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    Scene& scene = Service<Window>::get().scene();

    if (evt.action == InputAction::Pressed || evt.action == InputAction::Repeated)
    {
        // - Move camera -
        {
            glm::vec3 dir(0, 0, 0);

            switch (evt.key)
            {
            case 'D': dir.x = -1.0f; break;
            case 'A': dir.x = +1.0f; break;

            case 'W': dir.y = -1.0f; break;
            case 'S': dir.y = +1.0f; break;

            case 'Q': dir.z = +1.0f; break;
            case 'E': dir.z = -1.0f; break;
            }

            if (dir != glm::vec3(0, 0, 0))
            {
                m_theta += 0.01f * dir.x;
                m_distance += 0.05f * dir.y;

                scene.camera().position.x = m_distance * sin(m_theta);
                scene.camera().position.y = m_distance * cos(m_theta);
                scene.camera().position.z += 0.05f * dir.z;
            }
        }

        // - State -
        {
            switch (evt.key)
            {
            case KeyCode::Escape: wantQuit = true;
            }
        }
    }
}
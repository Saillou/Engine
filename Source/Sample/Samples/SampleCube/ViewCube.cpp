#include "ViewCube.hpp"

#include <Engine/Graphic/Window.hpp>
#include <Engine/Framework/Service.hpp>

ViewCube::ViewCube()
{
    // Root events
    _subscribe(&ViewCube::_update);

    // Materials
    glm::vec4 stone = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
    glm::vec4 paper = glm::vec4(0.5f, 0.5f, 0.5f, 0.4f);
    glm::vec4 glass = glm::vec4(0.3f, 1.0f, 1.0f, 0.5f);

    // - Ground -
    {
        DrawComponent draw;
        draw.type = DrawComponent::Solid;

        BodyComponent body;
        body.model    = Model::Load(Model::SimpleShape::Cube);
        body.material = stone;
        body.transform.local = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 5.0f, 0.01f));
        body.transform.world = glm::mat4(1.0f);

        m_entities["Ground"].push_back(ECS::createEntity());
        ECS::addComponent(m_entities["Ground"].back(), body);
        ECS::addComponent(m_entities["Ground"].back(), draw);
    }

    // - Lantern -
    {
        m_entities["Lantern"] = {}; // filled in state update
    }

    // - Cubes -
    {
        const float n_size = 0.5f;
        const int n_side   = 2;

        for (int x = -n_side; x <= n_side; x++) {
            for (int y = -n_side; y <= n_side; y++) {
                for (int z = 0; z <= n_side; z++) {
                    DrawComponent draw;
                    draw.type = DrawComponent::Solid;

                    BodyComponent body;
                    body.model = Model::Load(Model::SimpleShape::Cube);
                    body.material = paper;
                    body.transform.local = glm::scale(glm::mat4(1.0f), glm::vec3(0.15f));
                    body.transform.world = glm::translate(glm::mat4(1.0f), glm::vec3(n_size * x, n_size * y, 0.1f + n_size * z));

                    m_entities["Cube"].push_back(ECS::createEntity());
                    ECS::addComponent(m_entities["Cube"].back(), body);
                    ECS::addComponent(m_entities["Cube"].back(), draw);
                }
            }
        }
    }
}

ViewCube::~ViewCube()
{
    for (auto v_entity : m_entities) {
        for (auto entity : v_entity.second) {
            ECS::destroyEntity(entity);
        }
    }
}

// Events
void ViewCube::_update(const CommonEvents::StateUpdated&)
{
    Scene& scene = Service<Window>::get().scene();

    // Create scene's lights if needed
    if (m_entities["Lantern"].size() != scene.lights.size()) 
    {
        for (auto entity : m_entities["Lantern"]) {
            ECS::destroyEntity(entity);
        }
        m_entities["Lantern"].clear();

        for (const Light& light: scene.lights) 
        {
            Entity lantern = ECS::createEntity();

            DrawComponent draw;
            draw.type = DrawComponent::Solid;

            BodyComponent body;
            body.model           = Model::Load(Model::SimpleShape::Sphere);
            body.material        = light.color;
            body.transform.local = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
            body.transform.world = glm::translate(glm::mat4(1.0f), light.position);

            ECS::addComponent(lantern, body);
            ECS::addComponent(lantern, draw);

            m_entities["Lantern"].push_back(lantern);
        }
    }
}

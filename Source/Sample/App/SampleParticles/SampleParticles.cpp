#include "SampleParticles.hpp"

#include <memory>
#include <iostream>

SampleParticles::SampleParticles() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = { };
    m_scene.camera().up         = glm::vec3(0, 1, 0);
    m_scene.camera().position   = glm::vec3(0.0f, 0.0f, 4.0f);
    m_scene.camera().direction  = glm::vec3(0, 0, 0);

    // Particles
    m_entities["particle"] = Entity(Entity::SimpleShape::Quad);
    m_entities["particle"].setPosesWithMaterials(
        { 
            glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.25f)), glm::vec3(-1.5f,0,0)),
            glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.25f)), glm::vec3(+1.5f,0,0)),
        },
        { 
            { glm::vec4(0.2f, 0.7f, 0.5f, 0.9f), false },
            { glm::vec4(0.2f, 0.5f, 0.7f, 0.9f), false },
        }
    );

    // Enable events
    _subscribe(&SampleParticles::_draw);
}

SampleParticles::~SampleParticles() {
    m_scene.camera().up = glm::vec3(0, 0, 1);
}

// Events
void SampleParticles::_draw(const SceneEvents::Draw&) 
{
    // Draw items
    Renderer& render = m_scene.renderer();

    for (auto& entity : m_entities) {
        render.draw(Render::DrawType::Particle, entity.second);
    }
}

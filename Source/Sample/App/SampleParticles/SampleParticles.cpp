#include "SampleParticles.hpp"

#include <memory>
#include <iostream>
#include <random>

// Random engine
static std::default_random_engine gen;
static std::uniform_real_distribution<float> dstr_one(0.0f, 1.0f);

// Particles
SampleParticles::SampleParticles() :
    m_scene(Service<Window>::get().scene())
{
    // Scene
    m_scene.lights() = { };
    m_scene.camera().up        = glm::vec3(0, 1, 0);
    m_scene.camera().position  = glm::vec3(0.0f, 0.0f, +4.0f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Particles shape
    m_entities["particle"] = Entity(Entity::SimpleShape::Quad);
    m_entities["particle"].localMaterial().diffuse_color = glm::vec4(0.2f, 1.0f, 0.9f, 0.9f);

    // Enable events
    _subscribe(&SampleParticles::_draw);

    // Go
    m_timer.tic();
}

SampleParticles::~SampleParticles() {
    m_scene.camera().up = glm::vec3(0, 0, 1);
}

// Events
void SampleParticles::_draw(const SceneEvents::Draw&) 
{
    float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f;

    // Update particules
    m_entities["particle"].poses().clear();

    for (_particle& p: _particles) {
        p.pose = glm::translate(p.pose, p.speed * dt_ms);
        m_entities["particle"].poses().push_back(p.pose);
    }

    // Draw items
    Renderer& render = m_scene.renderer();

    for (auto& entity : m_entities) {
        render.draw(Render::DrawType::Particle, entity.second);
    }

    // Prepare next
    _generate_particle();
    while (_particles.size() > 1'000'000)
        _particles.pop_front();

    m_timer.tic();
}

// Methods
void SampleParticles::_generate_particle() {
    _particle particle;

    // Positions
    constexpr float x_range = 700.0f;
    constexpr float y_range = 10.0f;
    constexpr float z_start = -900.0f;

    particle.pose = glm::translate(glm::mat4(1.0f), glm::vec3(
        x_range * (dstr_one(gen) - 0.5f),
        y_range * (dstr_one(gen) - 0.5f),
        z_start
    ));

    // Speed
    constexpr float v_max = 10.0f;

    particle.speed = glm::vec3(
        0.0f, 0.0f, v_max * dstr_one(gen)
    );

    // Boum
    _particles.push_back(particle);
}

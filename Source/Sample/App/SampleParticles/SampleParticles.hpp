#pragma once

#include "../Sample.hpp"

struct SampleParticles : 
    public Sample 
{
     SampleParticles();
    ~SampleParticles();

private:
    // Base
    struct _particle {
        glm::mat4 pose;
        glm::vec3 speed;
    };

    // Events
    void _draw(const SceneEvents::Draw&);

    // Methods
    void _generate_particle();

    // Members
    Scene& m_scene;
    Timer::Chronometre m_timer;
    std::unordered_map<std::string, Entity> m_entities;

    std::deque<_particle> _particles;
};

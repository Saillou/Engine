#pragma once

#include "../Sample.hpp"

struct SampleParticles : 
    public Sample 
{
     SampleParticles();
    ~SampleParticles();

private:
    // Base
    struct _conditions {
        size_t number_limit;
        glm::vec3 p_start;
        glm::vec3 p_range;
        glm::vec3 v_dir;
        float v_max;
    };
    struct _particle {
        glm::mat4 pose;
        glm::vec3 speed;
    };

    // Events
    void _draw(const SceneEvents::Draw&);

    // Methods
    void _generate_particle(std::deque<_particle>& container, const _conditions& conditions);
    void _update_particle(std::deque<_particle>& container, std::vector<Pose>& entity_poses);

    void _add_custom_shader(const std::string& name);
    void _create_shader(const std::string& name);

    // Members
    Scene& m_scene;
    Timer::Chronometre m_timer;
    std::unordered_map<std::string, Entity> m_entities;
    std::unordered_map<std::string, Shader> m_shaders;

    std::deque<_particle> _particles;
    std::deque<_particle> _particles_custom;
};

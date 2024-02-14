#pragma once

#include "../Sample.hpp"

#include <deque>

struct SampleSnow : 
    public Sample 
{
     SampleSnow();

private:
    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _draw(const SceneEvents::Draw&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Methods
    void _generate_flakes();
    void _compute_physics(float delta_time_seconds);
    void _update_models();

    bool _is_flake_colliding(const glm::mat4& flake_pose) const;

    void _draw_hitbox(const std::string& model_name);

    // Members
    Scene& m_scene;
    Timer::Chronometre m_timer;
    std::unordered_map<std::string, Model::Ref> m_models;

    float m_cam_theta = 0.0f;
    float m_cam_distance = -7.0f;

    struct _Flake {
        glm::mat4 pose = glm::mat4(1.0f);
        size_t id      = 0;
        bool moving    = true;
    };
    std::deque<_Flake> m_flakes;
};

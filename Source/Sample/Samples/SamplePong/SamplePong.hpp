#pragma once

#include "../Sample.hpp"

struct SamplePong : 
    public Sample 
{
    SamplePong();

private:
    // Methods
    void _draw();
    void _physics();

    // Events
    void _update(const SceneEvents::Draw&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Scene
    Scene& m_scene;
    std::unordered_map<std::string, Entity> m_entities;

    // Game elements
    struct _Player {
        inline static const std::string Entity_Name = "player";
        inline static const float Speed = 0.05f;

        glm::vec3 pos = { 0, 0, 0 };
    };

    struct _Ball {
        inline static const std::string Entity_Name = "ball";

        glm::vec3 pos = { 0, 0, 0 };
    };

    _Ball   m_ball;
    _Player m_player_human;
    _Player m_player_ia;

    // State
    Timer::Chronometre m_timer;

    // Camera
    float m_distance = -3.5f;
    float m_theta = 0.0f;
};
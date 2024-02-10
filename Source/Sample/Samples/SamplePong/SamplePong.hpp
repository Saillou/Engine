#pragma once

#include "../Sample.hpp"

struct SamplePong : 
    public Sample 
{
    SamplePong();

private:
    // Events
    void _draw(const SceneEvents::Draw&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Methods
    // ..

    // Scene
    Scene& m_scene;
    Timer::Chronometre m_timer;
    std::unordered_map<std::string, Entity> m_entities;

    // Camera
    float m_distance = -10.0f;
    float m_theta = 0.0f;
};

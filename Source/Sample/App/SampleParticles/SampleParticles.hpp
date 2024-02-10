#pragma once

#include "../Sample.hpp"

struct SampleParticles : 
    public Sample 
{
    SampleParticles();
    ~SampleParticles();

private:
    // Events
    void _draw(const SceneEvents::Draw&);

    // Methods
    
    // Members
    Scene& m_scene;
    std::unordered_map<std::string, Entity> m_entities;
};

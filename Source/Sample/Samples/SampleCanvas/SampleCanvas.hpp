#pragma once

#include "../Sample.hpp"

struct SampleCanvas :
    public Sample
{
    SampleCanvas();

private:  
    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Scene
    Scene& m_scene;

    // State
    Timer::Chronometre m_timer;
};

#pragma once

#include "../Sample.hpp"

#include "SceneElements/Grid.hpp"
#include "SceneElements/Food.hpp"
#include "SceneElements/Monster.hpp"

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

    Grid    m_grid;
    Monster m_monster;

    // State
    Timer::Chronometre m_timer;
};

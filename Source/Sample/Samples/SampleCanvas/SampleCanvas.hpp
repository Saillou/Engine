#pragma once

#include "../Sample.hpp"

#include "SceneElements/Grid.hpp"
#include "SceneElements/Food.hpp"
#include "SceneElements/Monster.hpp"

#include <memory>
#include <vector>

struct SampleCanvas :
    public Sample
{
    SampleCanvas();

private:  
    // Methods
    void _generateFood();
    void _consumeFood(Entity foodId);

    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Scene
    Scene& m_scene;

    Grid    m_grid;
    Monster m_monster;
    std::vector<std::shared_ptr<Food>> m_foods;

    // State
    Timer::Chronometre m_timer;
};

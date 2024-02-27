#pragma once

#include "../Sample.hpp"

#include "SceneElements/Grid.hpp"
#include "SceneElements/Food.hpp"
#include "SceneElements/Monster.hpp"

#include "UiElements/LevelInfo.hpp"
#include "UiElements/SkillMenu.hpp"
#include "UiElements/TextBubble.hpp"

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

    // Members
    Scene& m_scene;

    // 3D-elements
    Grid    m_grid;
    Monster m_monster;
    std::vector<std::shared_ptr<Food>> m_foods;

    // 2D-elements
    SkillMenu  m_skills;
    TextBubble m_bubble;
    LevelInfo  m_level_info;

    // State
    Timer::Chronometre m_timer;
};

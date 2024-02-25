#pragma once

#include "../Sample.hpp"

#include "Actors/Ball.hpp"
#include "Actors/Brick.hpp"
#include "Actors/Field.hpp"
#include "Actors/Player.hpp"
#include "Actors/Wall.hpp"

struct SampleBreaker :
    public Sample
{
    SampleBreaker();

private:  
    void _init_scene();
    void _physics(float dt_ms);
    void _destroy_brick(Entity entity);

    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Scene
    Scene& m_scene;

    Field  m_field;
    Player m_player;
    Ball   m_ball;
    Wall   m_wall_left;
    Wall   m_wall_right;
    std::unordered_map<Entity, Brick> m_bricks;

    // State
    Timer::Chronometre m_timer;
};

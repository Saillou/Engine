#pragma once

#include "../Sample.hpp"
#include <Engine/Framework/Helper/ManagedEntity.hpp>

#include "PongActors.hpp"
#include "UiPong.hpp"

struct SamplePong :
    public Sample
{
    SamplePong();

private:
    // Methods
    void _init_game_elements();
    void _create_entities();
    void _update_players();

    ManagedEntity& _create_entity(const std::string& category, Model::Ref);

    void _ia();
    void _apply_actions(_Player&);
    void _physics(float dt_ms);

    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Scene
    Scene& m_scene;
    std::unordered_map<std::string, std::vector<SharedEntity>> m_entities;

    _Ball   m_ball;
    _Player m_player_human;
    _Player m_player_ia;
    _Wall   m_wall_1;
    _Wall   m_wall_2;

    // State
    UiPong m_ui;
    Timer::Chronometre m_timer;

    // Camera
    float m_distance = -3.5f;
    float m_theta = 0.0f;
};

#pragma once

#include "../../_imgui/imgui.h"
#include "../Sample.hpp"
#include <Engine/Framework/Helper/ManagedEntity.hpp>

struct SampleSnow : 
    public Sample 
{
    SampleSnow();

private:
    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Methods
    ManagedEntity& _create_entity(const std::string& category, Model::Ref);
    void _destroy_entity(const std::string& category, Entity);

    void _generate_flakes();
    void _compute_physics(float delta_time_seconds);

    bool _is_flake_colliding(const glm::mat4& flake_pose, bool accurate_check) const;

    // Members
    Scene& m_scene;
    Timer::Chronometre m_timer;
    std::unordered_map<std::string, std::vector<SharedEntity>> m_entities;

    float m_cam_theta = 0.0f;
    float m_cam_distance = -4.0f;

    struct _Flake {
        Entity id       = 0;
        glm::mat4& pose = glm::mat4(1.0f);
        bool accurate   = false;
        bool moving     = true;
    };
    std::deque<_Flake> m_flakes;
    Model::Ref m_model_flake = nullptr;
    glm::mat4 m_flake_local  = glm::mat4(1.0f);

    // -- Ui --
    struct Ui
    {
        void show();

#ifdef _DEBUG 
        bool loop = false;
        int flakes_number = 50;
#else 
        bool loop = true;
        int flakes_number = 1500;
#endif
    } m_ui;
};

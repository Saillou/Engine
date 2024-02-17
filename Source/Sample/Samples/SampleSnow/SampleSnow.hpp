#pragma once

#include "../../_imgui/imgui.h"
#include "../Sample.hpp"

struct SampleSnow : 
    public Sample 
{
     SampleSnow();
     ~SampleSnow();

private:
    // Events
    void _update(const CommonEvents::StateUpdated&);
    void _draw(const SceneEvents::Draw&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Methods
    void _add_shaders(const std::string&);
    Shader& _create_triangle_shader(Shader&);
    void _push_triangle_mesh(std::unique_ptr<Model::Node>&, const PrimitiveHelper::Triangle&);

    void _generate_flakes();
    void _compute_physics(float delta_time_seconds);
    void _update_models();

    bool _is_flake_colliding(const glm::mat4& flake_pose, bool accurate_check) const;

    void _draw_debug(const std::string& model_name);


    // Members
    Scene& m_scene;
    Timer::Chronometre m_timer;
    std::unordered_map<std::string, Model::Ref> m_models;
    std::unordered_map<std::string, Shader> m_shaders;

    float m_cam_theta = 0.0f;
    float m_cam_distance = -4.0f;

    struct _Flake {
        glm::mat4 pose = glm::mat4(1.0f);
        size_t id      = 0;
        bool accurate  = false;
        bool moving    = true;
    };
    std::deque<_Flake> m_flakes;

    // -- Ui --
    struct Ui
    {
        void show();

        bool show_debug = false;

#ifdef _DEBUG 
        bool loop = false;
        int flakes_number = 50;
#else 
        bool loop = true;
        int flakes_number = 1500;
#endif
    } m_ui;
};

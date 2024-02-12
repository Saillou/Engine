#pragma once


#include "../Sample.hpp"
#include "UiTrain.hpp"
#include <unordered_map>

class SampleTrain : public Sample
{
public:
    SampleTrain();

private:
    void _compute_physics();
    void _drawScene();
    void _drawInfo();

    void _on_update(const SceneEvents::Draw&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // - Members -
    Timer::Chronometre m_timer;
    UiTrain m_menu;
    std::unordered_map<std::string, Model::Ref> m_models;

    struct {
        float distance = 1.0f;
        float theta    = 0.0f;
    } m_camera_data;

    // Train variables
    struct {
        glm::vec2 position = { 0, 0 };
        float linear_speed = 0.0f;
        float angle        = 0.0f;
        float angle_speed  = 0.0f;
    } m_player_data;

    Scene& m_scene;
};

#pragma once

#include "../Sample.hpp"
#include "UiBreaker.hpp"

#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/PlayerControllerSystem.h"
#include "ECS/Systems/HumanControllerSystem.h"
#include "ECS/Systems/ForceSystem.h"
#include "ECS/Systems/BounceSystem.h"
#include "ECS/Systems/CollideSystem.h"

struct Player;
struct Ball;

struct SampleBreaker : 
    public Sample 
{
    SampleBreaker();

    // Game elements
    struct _Player {
        enum class Action {
            None, Left, Right
        };

        inline static const std::string Entity_Name = "player";
        inline static const float MaxSpeed = 0.05f;

        glm::vec3 pos = { 0, 0, 0 };
        Action next_action = Action::None;
    };

    struct _Ball {
        inline static const std::string Entity_Name = "ball";

        glm::vec3 pos = { 0, 0, 0 };
        glm::vec3 speed = { 0, 0, 0 };
    };

    struct _Wall {
        inline static const std::string Entity_Name = "wall";

        glm::vec3 pos = { 0, 0, 0 };
    };

private:
    // Methods
    void initLight();
    void initCamera();
    void initECS();
    void initGameObjects();

    // Events
    void _update(const SceneEvents::Draw&);
    void _on_key_pressed(const CommonEvents::KeyPressed& evt);

    // Scene
    Scene& m_scene;
    std::unordered_map<std::string, Entity> m_entities;

    // State
    UiBreaker m_ui;
    Timer::Chronometre m_timer;

    // Camera
    float m_distance = -3.5f;
    float m_theta = 0.0f;

    std::shared_ptr<RenderSystem>           m_renderSystem;
    std::shared_ptr<PlayerControllerSystem> m_playerControllerSystem;
    std::shared_ptr<HumanControllerSystem>  m_humanControllerSystem;
    std::shared_ptr<ForceSystem>            m_forceSystem;
    std::shared_ptr<CollideSystem>          m_colliderSystem;
    std::shared_ptr<BounceSystem>           m_bounceSystem;

    std::unique_ptr<Player> m_player;
    std::unique_ptr<Ball>   m_ball;
};

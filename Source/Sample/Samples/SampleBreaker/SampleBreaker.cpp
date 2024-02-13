#include "SampleBreaker.hpp"

#include <Engine/Framework/Service.hpp>
#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Utils/Physic/Collider.hpp>

#include "ECS/Components/Transform.h"
#include "ECS/Components/RenderComponent.h"
#include "ECS/Components/HumanController.h"
#include "ECS/Components/PlayerController.h"
#include "ECS/Components/Force.h"
#include "ECS/Components/Bounce.h"
#include "ECS/Components/Collider.h"

#include "ECS/Systems/RenderSystem.h"

#include "Objects/Player.h"
#include "Objects/Ball.h"

SampleBreaker::SampleBreaker() :
    m_scene(Service<Window>::get().scene())
{
    initLight();
    initCamera();
    initECS();
    initGameObjects();

    // Enable events
    _subscribe(&SampleBreaker::_update);
    _subscribe(&SampleBreaker::_on_key_pressed);

    // Go
    m_timer.tic();
}

void SampleBreaker::initLight()
{
    m_scene.lights() = { {glm::vec3(-1.0f, -2.0f, 2.50f), glm::vec4(1.0f) } };
}

void SampleBreaker::initCamera()
{
    m_scene.camera().position = glm::vec3(0, -6.f, -6.f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);
}

void SampleBreaker::initECS()
{
    ECS::registerComponent<Breaker::Transform>();
    ECS::registerComponent<Breaker::RenderComponent>();
    ECS::registerComponent<PlayerController>();
    ECS::registerComponent<HumanController>();
    ECS::registerComponent<Force>();
    ECS::registerComponent<Bounce>();
    ECS::registerComponent<ColliderComponent>();

    m_renderSystem              = ECS::registerSystem<RenderSystem>();
    m_playerControllerSystem    = ECS::registerSystem<PlayerControllerSystem>();
    m_humanControllerSystem     = ECS::registerSystem<HumanControllerSystem>();
    m_forceSystem               = ECS::registerSystem<ForceSystem>();
    m_bounceSystem              = ECS::registerSystem<BounceSystem>();
    m_colliderSystem            = ECS::registerSystem<CollideSystem>();

    m_renderSystem->init();
    m_playerControllerSystem->init();
    m_humanControllerSystem->init();
    m_forceSystem->init();
    m_bounceSystem->init();
    m_colliderSystem->init();
}

void SampleBreaker::initGameObjects()
{
    m_ball = std::make_unique<Ball>(glm::vec3(0.f,0.f,1.f));
    m_ball->setForce({ 0.0f, 0.0f, -0.5f });

    m_player = std::make_unique<Player>(glm::vec3(0.0f, 0.0f, -2.0f));
    m_player->enableControls();
}

// Events
void SampleBreaker::_update(const SceneEvents::Draw&)
{
    const float dt_ms = m_timer.elapsed<Timer::microsecond>() / 1000.0f / 1000.f;

    if (m_playerControllerSystem) {
        m_playerControllerSystem->update(dt_ms);
    }
    
    if (m_colliderSystem) {
        m_colliderSystem->update(dt_ms);
    }

    if (m_bounceSystem) {
        m_bounceSystem->update(dt_ms);
    }

    if (m_forceSystem) {
        m_forceSystem->update(dt_ms);
    }

    if (m_renderSystem) {
        m_renderSystem->update(m_scene.renderer());
    }

    m_ui.show();
    m_timer.tic();
}

void SampleBreaker::_on_key_pressed(const CommonEvents::KeyPressed& evt) {
    Scene& scene = Service<Window>::get().scene();

    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    // - Move player -
    {
        if (m_humanControllerSystem) {
            m_humanControllerSystem->update(evt.key);
        }
    }

    // - Move camera -
    {
        glm::vec3 dir(0, 0, 0);

        switch (evt.key) {
            case 'D': dir.x = -1.0f; break;
            case 'A': dir.x = +1.0f; break;

            case 'W': dir.y = -1.0f; break;
            case 'S': dir.y = +1.0f; break;

            case 'Q': dir.z = +1.0f; break;
            case 'E': dir.z = -1.0f; break;       

            case 'Y':
                if (m_player)
                    m_ball->setForce({ 0.f,0.f,+0.5f });
                break;
            case 'U':
                if (m_player)
                    m_ball->setForce({0.f,0.f,0.f});
                break;
            case 'I':
                if (m_player)
                    m_ball->setForce({ 0.f,0.f,-0.5f });
                break;
        }

        if (dir != glm::vec3(0, 0, 0)) {
            m_theta    += 0.01f * dir.x;
            m_distance += 0.05f * dir.y;

            scene.camera().position.x = m_distance * sin(m_theta);
            scene.camera().position.y = m_distance * cos(m_theta);
            scene.camera().position.z += 0.05f * dir.z;
        }
    }

    // - State -
    {
        switch (evt.key) {
            case KeyCode::Escape: wantQuit = true;
        }
    }
}
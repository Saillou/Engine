#include "PlayerControllerSystem.h"

#include <Engine/Framework/Core/ECS.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/PlayerController.h"

void PlayerControllerSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<PlayerController>());
    signature.set(ECS::getComponentType<Breaker::Transform>());

    ECS::setSystemSignature<PlayerControllerSystem>(signature);
}

void PlayerControllerSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        PlayerController& controller = ECS::getComponent<PlayerController>(entity);
        Breaker::Transform& transform = ECS::getComponent<Breaker::Transform>(entity);

        switch (controller.action)
        {
        case PlayerController::Action::Left:
            transform.position.x -= controller.speed;
            break;

        case PlayerController::Action::Right:
            transform.position.x += controller.speed;
            break;
        }

        controller.action = PlayerController::Action::None;
    }
}
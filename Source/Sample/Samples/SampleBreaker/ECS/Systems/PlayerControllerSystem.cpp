#include "PlayerControllerSystem.h"

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/PlayerController.h"

void PlayerControllerSystem::init()
{
    Thomas::Signature signature;

    signature.set(Thomas::ECS::getComponentType<PlayerController>());
    signature.set(Thomas::ECS::getComponentType<Thomas::Transform>());

    Thomas::ECS::setSystemSignature<PlayerControllerSystem>(signature);
}

void PlayerControllerSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        PlayerController& controller = Thomas::ECS::getComponent<PlayerController>(entity);
        Thomas::Transform& transform = Thomas::ECS::getComponent<Thomas::Transform>(entity);

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
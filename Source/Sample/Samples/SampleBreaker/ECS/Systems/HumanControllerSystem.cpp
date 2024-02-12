#include "HumanControllerSystem.h"

#include "Engine/Events/Common.hpp"

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/HumanController.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/PlayerController.h"

void HumanControllerSystem::init()
{
    Thomas::Signature signature;

    signature.set(Thomas::ECS::getComponentType<PlayerController>());
    signature.set(Thomas::ECS::getComponentType<HumanController>());

    Thomas::ECS::setSystemSignature<HumanControllerSystem>(signature);
}

void HumanControllerSystem::update(const int key) 
{
    for (auto& entity : m_entities)
    {
        PlayerController& controller = Thomas::ECS::getComponent<PlayerController>(entity);

        switch (key) 
        {
            case KeyCode::ArrowLeft:  controller.action = PlayerController::Action::Left; break;
            case KeyCode::ArrowRight: controller.action = PlayerController::Action::Right; break;
        }
    }
}
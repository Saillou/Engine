#include "HumanControllerSystem.h"

#include <Engine/Events/Common.hpp>
#include <Engine/Framework/Core/ECS.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/HumanController.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/PlayerController.h"

void HumanControllerSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<PlayerController>());
    signature.set(ECS::getComponentType<HumanController>());

    ECS::setSystemSignature<HumanControllerSystem>(signature);
}

void HumanControllerSystem::update(const int key) 
{
    for (auto& entity : m_entities)
    {
        PlayerController& controller = ECS::getComponent<PlayerController>(entity);

        switch (key) 
        {
            case KeyCode::ArrowLeft:  controller.action = PlayerController::Action::Left; break;
            case KeyCode::ArrowRight: controller.action = PlayerController::Action::Right; break;
        }
    }
}
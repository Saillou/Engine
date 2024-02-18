#include "ForceSystem.h"

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Core/ECSTypes.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Force.h"

void ForceSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<Force>());
    signature.set(ECS::getComponentType<Breaker::Transform>());

    ECS::setSystemSignature<ForceSystem>(signature);
}

void ForceSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        Force& force                    = ECS::getComponent<Force>(entity);
        Breaker::Transform& transform    = ECS::getComponent<Breaker::Transform>(entity);

        transform.position += dt * force.force;
    }
}
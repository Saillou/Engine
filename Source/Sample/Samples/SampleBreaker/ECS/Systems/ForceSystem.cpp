#include "ForceSystem.h"

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Force.h"

void ForceSystem::init()
{
    Thomas::Signature signature;

    signature.set(Thomas::ECS::getComponentType<Force>());
    signature.set(Thomas::ECS::getComponentType<Thomas::Transform>());

    Thomas::ECS::setSystemSignature<ForceSystem>(signature);
}

void ForceSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        Force& force                    = Thomas::ECS::getComponent<Force>(entity);
        Thomas::Transform& transform    = Thomas::ECS::getComponent<Thomas::Transform>(entity);

        transform.position += dt * force.force;
    }
}
#include "BounceSystem.h"

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/Bounce.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Collider.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Force.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"

void BounceSystem::init()
{
    Thomas::Signature signature;

    signature.set(Thomas::ECS::getComponentType<ColliderComponent>());
    signature.set(Thomas::ECS::getComponentType<Bounce>());
    signature.set(Thomas::ECS::getComponentType<Force>());
    signature.set(Thomas::ECS::getComponentType<Thomas::Transform>());

    Thomas::ECS::setSystemSignature<BounceSystem>(signature);
}

void BounceSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        ColliderComponent& collider = Thomas::ECS::getComponent<ColliderComponent>(entity);
        if (!collider.hasCollision)
            continue;

        collider.hasCollision = false;

        Force& forceComponent               = Thomas::ECS::getComponent<Force>(entity);
        Thomas::Transform& transform        = Thomas::ECS::getComponent<Thomas::Transform>(entity);
        Thomas::Transform& anotherTransform = Thomas::ECS::getComponent<Thomas::Transform>(collider.collidedEntity);

        if (transform.position.z > anotherTransform.position.z)
        {
            //forceComponent.force.z *= -1;
        }
    }
}
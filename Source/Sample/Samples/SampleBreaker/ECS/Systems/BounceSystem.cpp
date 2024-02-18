#include "BounceSystem.h"

#include <Engine/Framework/Core/ECS.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/Bounce.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Collider.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Force.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"

void BounceSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<ColliderComponent>());
    signature.set(ECS::getComponentType<Bounce>());
    signature.set(ECS::getComponentType<Force>());
    signature.set(ECS::getComponentType<Breaker::Transform>());

    ECS::setSystemSignature<BounceSystem>(signature);
}

void BounceSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        ColliderComponent& collider = ECS::getComponent<ColliderComponent>(entity);
        if (!collider.hasCollision)
            continue;

        collider.hasCollision = false;

        Force& forceComponent               = ECS::getComponent<Force>(entity);
        Breaker::Transform& transform        = ECS::getComponent<Breaker::Transform>(entity);
        Breaker::Transform& anotherTransform = ECS::getComponent<Breaker::Transform>(collider.collidedEntity);

        if (transform.position.z > anotherTransform.position.z)
        {
            //forceComponent.force.z *= -1;
        }
    }
}
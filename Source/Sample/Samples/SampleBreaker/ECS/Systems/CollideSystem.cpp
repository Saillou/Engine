#include "CollideSystem.h"

#include <stdio.h>

// -- debug
#include <Engine/Utils/Service.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Window.hpp>
// end debug

#include <Engine/Utils/Physic/Collider.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Core/ECS.h"

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Collider.h"

void CollideSystem::init()
{
    Thomas::Signature signature;

    signature.set(Thomas::ECS::getComponentType<ColliderComponent>());
    signature.set(Thomas::ECS::getComponentType<Thomas::Transform>());

    Thomas::ECS::setSystemSignature<CollideSystem>(signature);
}

void CollideSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        Thomas::Transform& transform    = Thomas::ECS::getComponent<Thomas::Transform>(entity);
        ColliderComponent& collider     = Thomas::ECS::getComponent<ColliderComponent>(entity);

        if (!collider.collisionBox)
            continue;

        if (collider.hasCollision)
            continue;

        collider.collisionBox->poses() = { {transform.getMat4()} };

        Entity hitbox(Entity::Cube);
        hitbox.localMaterial().diffuse_color = { 0.7f, 0.2f,0.2f,1.f };
        hitbox.poses() = {
            {
                transform.getMat4() *
                collider.collisionBox->model().localPose() *
                collider.collisionBox->model().root()->transform *
                collider.collisionBox->model().root()->meshes.front()->obb()
            }
        };

        
        auto& scene = Service<Window>::get().scene();
        scene.renderer().draw(Render::DrawType::Geometry, hitbox);

        for (auto& anotherEntity : m_entities)
        {
            if (entity == anotherEntity)
                continue;

            Thomas::Transform& anotherTransform = Thomas::ECS::getComponent<Thomas::Transform>(anotherEntity);

            // ignore far entities
            if (glm::distance(transform.position, anotherTransform.position) > collider.searchDistance)
                continue;

            ColliderComponent& anotherCollider = Thomas::ECS::getComponent<ColliderComponent>(entity);

            if (!anotherCollider.collisionBox)
                continue;

            anotherCollider.collisionBox->poses() = { {anotherTransform.getMat4()} };

            Entity hitboxanother(Entity::Cube);
            hitboxanother.localMaterial().diffuse_color = { 0.2f, 0.2f,0.7f,1.f };
            hitboxanother.poses() = {
                {
                    anotherTransform.getMat4() *
                    anotherCollider.collisionBox->model().localPose() *
                    anotherCollider.collisionBox->model().root()->transform *
                    anotherCollider.collisionBox->model().root()->meshes.front()->obb()
                }
            };

            scene.renderer().draw(Render::DrawType::Geometry, hitboxanother);

            const auto collisionPoint = Collider::CheckMultiple(
                *collider.collisionBox,         transform.getMat4(),
                *anotherCollider.collisionBox,  anotherTransform.getMat4()
            );

            if (collisionPoint.has_value()) 
            {
                Entity point(Entity::Sphere);
                point.localMaterial().diffuse_color = { 0.7f, 0.2f,0.2f,1.f };
                point.localPose() = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));
                for (auto p : collisionPoint.value())
                {
                    point.poses().push_back(glm::translate(glm::mat4(1.f), p));
                }

                scene.renderer().draw(Render::DrawType::Geometry, point);

                // With this implementation ColliderComponent can have collision only with 1 Entity
                collider.collidedEntity = anotherEntity;
                collider.hasCollision = true;

                break; // we expect only one collision
            }
        }
    }
}
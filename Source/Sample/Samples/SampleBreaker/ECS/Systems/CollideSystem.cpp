#include "CollideSystem.h"

#include <stdio.h>

// -- debug
#include <Engine/Framework/Service.hpp>
#include <Engine/Graphic/Base/Scene.hpp>
#include <Engine/Graphic/Window.hpp>
// end debug

#include <Engine/Utils/Physic/Collider.hpp>
#include <Engine/Framework/Core/ECS.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Components/Transform.h"
#include "Sample/Samples/SampleBreaker/ECS/Components/Collider.h"

void CollideSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<ColliderComponent>());
    signature.set(ECS::getComponentType<Breaker::Transform>());

    ECS::setSystemSignature<CollideSystem>(signature);
}

void CollideSystem::update(float dt)
{
    for (auto& entity : m_entities)
    {
        Breaker::Transform& transform    = ECS::getComponent<Breaker::Transform>(entity);
        ColliderComponent& collider     = ECS::getComponent<ColliderComponent>(entity);

        if (!collider.collisionBox)
            continue;

        if (collider.hasCollision)
            continue;

        collider.collisionBox->poses = { {transform.getMat4()} };

        Model::Ref hitbox = Model::Create(Model::Cube);
        hitbox->localMaterial.diffuse_color = { 0.7f, 0.2f,0.2f,1.f };
        hitbox->poses = {
            {
                transform.getMat4() *
                collider.collisionBox->localPose *
                collider.collisionBox->root->transform *
                collider.collisionBox->root->meshes.front()->obb()
            }
        };

        
        auto& scene = Service<Window>::get().scene();
        scene.renderer().draw(Render::DrawType::Geometry, hitbox);

        for (auto& anotherEntity : m_entities)
        {
            if (entity == anotherEntity)
                continue;

            Breaker::Transform& anotherTransform = ECS::getComponent<Breaker::Transform>(anotherEntity);

            // ignore far entities
            if (glm::distance(transform.position, anotherTransform.position) > collider.searchDistance)
                continue;

            ColliderComponent& anotherCollider = ECS::getComponent<ColliderComponent>(entity);

            if (!anotherCollider.collisionBox)
                continue;

            anotherCollider.collisionBox->poses = { {anotherTransform.getMat4()} };

            Model::Ref hitboxanother = Model::Create(Model::Cube);
            hitboxanother->localMaterial.diffuse_color = { 0.2f, 0.2f,0.7f,1.f };
            hitboxanother->poses = {
                {
                    anotherTransform.getMat4() *
                    anotherCollider.collisionBox->localPose *
                    anotherCollider.collisionBox->root->transform *
                    anotherCollider.collisionBox->root->meshes.front()->obb()
                }
            };

            scene.renderer().draw(Render::DrawType::Geometry, hitboxanother);

            const auto collisionPoint = Collider::CheckHitboxes(
                collider.collisionBox,         transform.getMat4(),
                anotherCollider.collisionBox,  anotherTransform.getMat4()
            );

            if (collisionPoint.has_value()) 
            {
                Model::Ref point = Model::Create(Model::Sphere);
                point->localMaterial.diffuse_color = { 0.7f, 0.2f,0.2f,1.f };
                point->localPose = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));
                point->poses.push_back(glm::translate(glm::mat4(1.f), collisionPoint.value()));

                scene.renderer().draw(Render::DrawType::Geometry, point);

                // With this implementation ColliderComponent can have collision only with 1 Entity
                collider.collidedEntity = anotherEntity;
                collider.hasCollision = true;

                break; // we expect only one collision
            }
        }
    }
}
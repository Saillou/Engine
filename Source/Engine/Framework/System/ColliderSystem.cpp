#include "ColliderSystem.hpp"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

void ColliderSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());
    signature.set(ECS::getComponentType<CollideComponent>());

    ECS::setSystemSignature<ColliderSystem>(signature);
}

void ColliderSystem::check(Entity entityChecked)
{
    const BodyComponent& bodyChecked   = ECS::getComponent<BodyComponent>(entityChecked);
    CollideComponent& collide_result   = ECS::getComponent<CollideComponent>(entityChecked);

    // Clear previous computation
    collide_result.hit_entities.clear();
    collide_result.hit_positions.clear();
    collide_result.is_colliding = false;

    if (collide_result.type == CollideComponent::Disabled)
        return;

    // Check against all entities
    for (Entity entity : m_entities) {
        // Avoid tautologism
        if (entity == entityChecked)
            continue;

        // Entity check
        const BodyComponent& body = ECS::getComponent<BodyComponent>(entity);

        // - Check depending on type -
        std::optional<std::vector<glm::vec3>> collision_points;

        if (collide_result.type & CollideComponent::Multiple) {
            if (collide_result.type & CollideComponent::Accurate) {
                collision_points = Collider::CheckAccurateMultiple(
                    body.model,        body.transform.get(),
                    bodyChecked.model, bodyChecked.transform.get()
                );
            }
            else {
                collision_points = Collider::CheckHitboxesMultiple(
                    body.model,        body.transform.get(),
                    bodyChecked.model, bodyChecked.transform.get()
                );
            }
        }
        else {
            std::optional<glm::vec3> collision_point;
            if (collide_result.type & CollideComponent::Accurate) 
            {
                collision_point = Collider::CheckAccurate(
                    body.model,        body.transform.get(),
                    bodyChecked.model, bodyChecked.transform.get()
                );
            }
            else 
            {
                collision_point = Collider::CheckHitboxes(
                    body.model,        body.transform.get(),
                    bodyChecked.model, bodyChecked.transform.get()
                );
            }

            if (!collision_point.has_value())
                continue;

            collision_points = {
                collision_point.value() 
            };
        }

        // Got something?
        if (!collision_points.has_value())
            continue;

        // Deal with results
        collide_result.is_colliding  = true;
        collide_result.hit_entities  = { entity };
        collide_result.hit_positions = collision_points.value();

        // Stop searching (TODO: maybe shall continue depending on some flags...)
        return;
    }
}

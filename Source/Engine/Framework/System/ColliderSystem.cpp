#include "ColliderSystem.hpp"

void ColliderSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<BodyComponent>());
    signature.set(ECS::getComponentType<BatchComponent>());
    signature.set(ECS::getComponentType<CollideComponent>());

    ECS::setSystemSignature<ColliderSystem>(signature);
}

void ColliderSystem::update(Entity entityChecked)
{
    const BodyComponent& bodyChecked   = ECS::getComponent<BodyComponent>(entityChecked);
    const BatchComponent& batchChecked = ECS::getComponent<BatchComponent>(entityChecked);
    CollideComponent& collide_result   = ECS::getComponent<CollideComponent>(entityChecked);

    // Clear previous computation
    collide_result.hit_entities.clear();
    collide_result.hit_positions.clear();
    collide_result.is_colliding = false;

    if (collide_result.type == CollideComponent::Disabled)
        return;

    // TODO: change that because it's only test the first element of a batch
    const glm::mat4& checked_transform = batchChecked.transforms.front()* bodyChecked.localTransform;

    // Check against all entities
    for (Entity entity : m_entities) {
        // Avoid tautologism
        if (entity == entityChecked)
            continue;

        // Batch check
        const BatchComponent& batch = ECS::getComponent<BatchComponent>(entityChecked);
        const BodyComponent& body   = ECS::getComponent<BodyComponent>(entityChecked);

        for (const glm::mat4& world_transform : batch.transforms) 
        {
            const glm::mat4& transform = world_transform * body.localTransform;

            // - Check depending on type -
            std::optional<std::vector<glm::vec3>> collision_points;

            if (collide_result.type & CollideComponent::Multiple) {
                if (collide_result.type & CollideComponent::Accurate) {
                    collision_points = Collider::CheckAccurateMultiple(
                        body.model,        transform,
                        bodyChecked.model, checked_transform
                    );
                }
                else {
                    collision_points = Collider::CheckHitboxesMultiple(
                        body.model,        transform,
                        bodyChecked.model, checked_transform
                    );
                }
            }
            else {
                std::optional<glm::vec3> collision_point;
                if (collide_result.type & CollideComponent::Accurate) 
                {
                    collision_point = Collider::CheckAccurate(
                        body.model,        transform,
                        bodyChecked.model, checked_transform
                    );
                }
                else 
                {
                    collision_point = Collider::CheckHitboxes(
                        body.model,        transform,
                        bodyChecked.model, checked_transform
                    );
                }

                if (!collision_point.has_value())
                    continue;

                collision_points = { collision_point.value() };
            }

            // Got something
            if (!collision_points.has_value())
                continue;

            // Deal with results
            collide_result.is_colliding = true;
            collide_result.hit_entities = { entity };
            collide_result.hit_positions = collision_points.value();

            // Stop searching (TODO: maybe shall continue depending on some flags...)
            return;
        }
    }
}

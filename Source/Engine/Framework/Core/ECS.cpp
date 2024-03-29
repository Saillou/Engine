#include "ECS.hpp"

#include "../Component/BodyComponent.hpp"
#include "../Component/CastComponent.hpp"
#include "../Component/DrawComponent.hpp"
#include "../Component/CanvasComponent.hpp"
#include "../Component/FilterComponent.hpp"
#include "../Component/CollideComponent.hpp"
#include "../Component/ParticleComponent.hpp"

static bool initialized = false;

void ECS::init()
{
    if (initialized)
        return;

    ECS::registerComponent<BodyComponent>();
    ECS::registerComponent<CastComponent>();
    ECS::registerComponent<DrawComponent>();
    ECS::registerComponent<CanvasComponent>();
    ECS::registerComponent<FilterComponent>();
    ECS::registerComponent<CollideComponent>();
    ECS::registerComponent<ParticleComponent>();

    initialized = true;
}

Entity ECS::createEntity()
{
    return EntityManager::createEntity();
}

void ECS::destroyEntity(Entity entity)
{
    EntityManager::destroyEntity(entity);

    ComponentManager::entityDestroyed(entity);
    SystemManager::entityDestroyed(entity);
}

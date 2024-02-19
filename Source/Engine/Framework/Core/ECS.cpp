#include "ECS.hpp"

#include "../Component/BodyComponent.hpp"
#include "../Component/DrawComponent.hpp"
#include "../Component/BatchComponent.hpp"
#include "../Component/CollideComponent.hpp"

static bool initialized = false;

void ECS::init()
{
    if (initialized)
        return;

    ECS::registerComponent<BodyComponent>();
    ECS::registerComponent<DrawComponent>();
    ECS::registerComponent<BatchComponent>();
    ECS::registerComponent<CollideComponent>();

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

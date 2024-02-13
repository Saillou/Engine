#include "ECS.hpp"

void ECS::init()
{
    // init all other managers here, for now not needed
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

#include "EntityManager.hpp"

#include <iostream>
#include <cassert>

EntityManager::EntityManager(): 
    m_signatures({}), 
    m_livingEntityCount(0)
{
    for (Entity entity = 0; entity < MAX_ENTITIES; entity++) {
        m_availableEntities.push(entity);
    }
}

EntityManager& EntityManager::get() {
    static EntityManager manager;
    return manager;
}

Entity EntityManager::createEntity() {
    EntityManager& manager = get();

    assert(manager.m_livingEntityCount < MAX_ENTITIES && "Too many entities in existance.");

    Entity entity = manager.m_availableEntities.front();
    manager.m_availableEntities.pop();
    ++manager.m_livingEntityCount;

    return entity;
}

void EntityManager::destroyEntity(Entity entity) {
    EntityManager& manager = get();

    assert(entity < MAX_ENTITIES && "Entity is out of range.");

    manager.m_signatures[entity].reset();
    manager.m_availableEntities.push(entity);
    --manager.m_livingEntityCount;
}

void EntityManager::setSignature(Entity entity, Signature signature) {
    EntityManager& manager = get();

    assert(entity < MAX_ENTITIES && "Entity is out of range.");

    manager.m_signatures[entity] = signature;
}

Signature EntityManager::getSignature(Entity entity) {
    EntityManager& manager = get();

    assert(entity < MAX_ENTITIES && "Entity is out of range.");

    return manager.m_signatures[entity];
}
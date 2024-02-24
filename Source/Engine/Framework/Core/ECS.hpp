#pragma once

#include <memory>

#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"
#include "ECSTypes.hpp"

struct ECS
{
    static void init();

    static Entity createEntity();
    static void destroyEntity(Entity entity);

    template<typename T>
    static void registerComponent();

    template<typename T>
    static void addComponent(Entity entity, T component);

    template<typename T>
    static void removeComponent(Entity entity);

    template<typename T>
    static bool hasComponent(Entity entity);

    template<typename T>
    static T& getComponent(Entity entity);

    template<typename T>
    static ComponentType getComponentType();

    template<typename T, typename... Args>
    static std::shared_ptr<T> registerSystem(Args&&... args);

    template<typename T>
    static void setSystemSignature(Signature signature);

private:
    ECS() {};
    ECS(const ECS& other) = delete;
    ECS(const ECS&& other) = delete;
};

template<typename T>
inline void ECS::registerComponent()
{
    ComponentManager::registerComponent<T>();
}

template<typename T>
inline void ECS::addComponent(Entity entity, T component)
{
    ComponentManager::addComponent<T>(entity, component);

    Signature signature = EntityManager::getSignature(entity);
    signature.set(ComponentManager::getComponentType<T>(), true);
    EntityManager::setSignature(entity, signature);

    SystemManager::entitySignatureChanged(entity, signature);
}

template<typename T>
inline void ECS::removeComponent(Entity entity)
{
    ComponentManager::removeComponent<T>(entity);

    Signature signature = EntityManager::getSignature(entity);
    signature.set(ComponentManager::getComponentType<T>(), false);
    EntityManager::setSignature(entity, signature);

    SystemManager::entitySignatureChanged(entity, signature);
}

template<typename T>
inline bool ECS::hasComponent(Entity entity)
{
    return ComponentManager::hasComponent<T>(entity);
}

template<typename T>
inline T& ECS::getComponent(Entity entity)
{
    return ComponentManager::getComponent<T>(entity);
}

template<typename T>
inline ComponentType ECS::getComponentType()
{
    return ComponentManager::getComponentType<T>();
}

template<typename T, typename... Args>
inline std::shared_ptr<T> ECS::registerSystem(Args&&... args)
{
    return SystemManager::registerSystem<T>(std::forward<Args>(args)...);
}

template<typename T>
inline void ECS::setSystemSignature(Signature signature)
{
    SystemManager::setSignature<T>(signature);
}

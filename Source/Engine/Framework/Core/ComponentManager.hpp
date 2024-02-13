#pragma once

#include <any>
#include <memory>
#include <unordered_map>

#include "ComponentArray.hpp"
#include "ECSTypes.hpp"


/*
*  ComponentManager is designed to control ComponentArray in a convinient way, it can do following:
*  - registry Component => assigns Component type (bitset shift), creates a shared_ptr<ComponentArray<T>>
*  - get Component type (bitset shift)
*  - add/remove/get Component to the Entity
*/

struct ComponentManager
{
    static ComponentManager& get();

    template<typename T>
    static void registerComponent();

    template<typename T>
    static ComponentType getComponentType();

    template<typename T>
    static void addComponent(Entity entity, T component);

    template<typename T>
    static void removeComponent(Entity entity);

    template<typename T>
    static T& getComponent(Entity entity);

    static void entityDestroyed(Entity entity);

private:
    ComponentManager() {};
    ComponentManager(const ComponentManager& other) = delete;
    ComponentManager(const ComponentManager&& other) = delete;

    std::unordered_map<const char*, ComponentType>                      m_componentTypes{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>>   m_componentArrays{};
    ComponentType m_nextComponentType{};

    template<typename T>
    std::shared_ptr<ComponentArray<T>> getComponentArray();
};

template<typename T>
inline void ComponentManager::registerComponent()
{
    ComponentManager& manager = get();
    const char* typeName = typeid(T).name();

    assert(manager.m_componentTypes.find(typeName) == manager.m_componentTypes.end() && "Registering component type more than once.");

    manager.m_componentTypes.insert({ typeName, manager.m_nextComponentType });
    manager.m_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

    ++manager.m_nextComponentType;
}

template<typename T>
inline ComponentType ComponentManager::getComponentType()
{
    ComponentManager& manager = get();
    const char* typeName = typeid(T).name();

    assert(manager.m_componentTypes.find(typeName) != manager.m_componentTypes.end() && "Component is not registered.");

    return manager.m_componentTypes[typeName];
}

template<typename T>
inline void ComponentManager::addComponent(Entity entity, T component)
{
    ComponentManager& manager = get();
    manager.getComponentArray<T>()->insertData(entity, component);
}

template<typename T>
inline void ComponentManager::removeComponent(Entity entity)
{
    ComponentManager& manager = get();
    manager.getComponentArray<T>()->removeData(entity);
}

template<typename T>
inline T& ComponentManager::getComponent(Entity entity)
{
    ComponentManager& manager = get();
    return manager.getComponentArray<T>()->getData(entity);
}

template<typename T>
inline std::shared_ptr<ComponentArray<T>> ComponentManager::getComponentArray()
{
    ComponentManager& manager = get();

    const char* typeName = typeid(T).name();

    assert(manager.m_componentTypes.find(typeName) != manager.m_componentTypes.end() && "Component is not registered.");

    return std::dynamic_pointer_cast<ComponentArray<T>>(manager.m_componentArrays[typeName]);
}

#pragma once

#include <cassert>
#include <memory>
#include <unordered_map>

#include "System.hpp"
#include "ECSTypes.hpp"

/*
*  SystemManager is in charge of maintainig a record of registered systems and their signatures.
*  Each time Entity is changing its Signature - callback is called to update a set of Entities.
*/

struct SystemManager
{
    static SystemManager& get();

    template<typename T>
    static std::shared_ptr<T> registerSystem();

    template<typename T>
    static void setSignature(Signature signature);

    static void entityDestroyed(Entity entity);
    static void entitySignatureChanged(Entity entity, Signature signature);

private:
    SystemManager() {};
    SystemManager(const SystemManager& other) = delete;
    SystemManager(const SystemManager&& other) = delete;

    std::unordered_map<const char*, Signature>                  m_signatures{};
    std::unordered_map<const char*, std::shared_ptr<System>>    m_systems{};
};

template<typename T>
inline std::shared_ptr<T> SystemManager::registerSystem()
{
    SystemManager& manager = get();

    const char* typeName = typeid(T).name();

    assert(manager.m_systems.find(typeName) == manager.m_systems.end() && "System is already registered.");

    auto system = std::make_shared<T>();
    manager.m_systems.insert({ typeName, system });

    return system;
}

template<typename T>
inline void SystemManager::setSignature(Signature signature)
{
    SystemManager& manager = get();

    const char* typeName = typeid(T).name();

    assert(manager.m_systems.find(typeName) != manager.m_systems.end() && "System is not registered.");

    manager.m_signatures.insert({ typeName, signature });
}
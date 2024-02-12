#include "SystemManager.hpp"

SystemManager& SystemManager::get()
{
    static SystemManager manager;
    return manager;
}

void SystemManager::entityDestroyed(Entity entity)
{
    SystemManager& manager = get();

    for (auto const& pair : manager.m_systems)
    {
        auto const& system = pair.second;
        system->m_entities.erase(entity);
        system->removeEntity(entity);
    }
}

void SystemManager::entitySignatureChanged(Entity entity, Signature signature)
{
    SystemManager& manager = get();

    for (auto const& pair : manager.m_systems)
    {
        auto const& type            = pair.first;
        auto const& system          = pair.second;
        auto const& systemSignature = manager.m_signatures[type];

        if ((signature & systemSignature) == systemSignature)
        {
            system->m_entities.insert(entity);
            system->addEntity(entity);
        }
        else
        {
            system->m_entities.erase(entity);
            system->removeEntity(entity);
        }
    }
}
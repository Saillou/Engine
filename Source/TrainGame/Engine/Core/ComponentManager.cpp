#include "ComponentManager.h"

namespace Thomas
{
    ComponentManager& ComponentManager::get()
    {
        static ComponentManager manager;
        return manager;
    }

    void ComponentManager::entityDestroyed(Entity entity)
    {
        ComponentManager& manager = get();

        for (auto const& pair : manager.m_componentArrays)
        {
            auto const& component = pair.second;

            component->entityDestroyed(entity);
        }
    }
} // namespace Thomas
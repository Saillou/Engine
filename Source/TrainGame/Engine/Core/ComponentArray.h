#pragma once

#include <array>
#include <cassert>
#include <unordered_map>

#include "Types.h"

namespace Thomas
{
    /*
    *  The goal of ComponentArray is to always be a packed array, meaning it has no holes.
    *  This class stores the values of the Component for the all Entities, it allows to:
    *  - add/remove/get Component data to the Entity
    */

    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray 
    {
    public:
        void insertData(Entity entity, T component);
        void removeData(Entity entity);

        T& getData(Entity entity);

        void entityDestroyed(Entity entity) override;

    private:
        std::array<T, MAX_ENTITIES>         m_componentArray;
        std::unordered_map<Entity, size_t>  m_entityToIndexMap;
        std::unordered_map<size_t, Entity>  m_indexToEntityMap;
        size_t                              m_size;
    };

    template<typename T>
    inline void ComponentArray<T>::insertData(Entity entity, T component)
    {
        if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
            return;

        m_entityToIndexMap[entity] = m_size;
        m_indexToEntityMap[m_size] = entity;
        m_componentArray[m_size] = component;

        ++m_size;
    }

    template<typename T>
    inline void ComponentArray<T>::removeData(Entity entity)
    {
        if (m_entityToIndexMap.find(entity) == m_entityToIndexMap.end())
            return;

        // Copy element at end into deleted element's place to maintain density
        const size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
        const size_t indexOfLastElement = m_size - 1;
        m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

        // Update map to point to moved spot
        const Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
        m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        m_entityToIndexMap.erase(entity);
        m_indexToEntityMap.erase(indexOfLastElement);

        --m_size;
    }

    template<typename T>
    inline T& ComponentArray<T>::getData(Entity entity)
    {
        assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Retrieving non-existent component.");

        return m_componentArray[m_entityToIndexMap[entity]];
    }

    template<typename T>
    void ComponentArray<T>::entityDestroyed(Entity entity)
    {
        if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
        {
            removeData(entity);
        }
    }
} // namespace Thomas
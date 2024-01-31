#pragma once

#include <array>
#include <queue>

#include "Types.h"

namespace Thomas
{
    /*
    *  EntityManager is responsible for:
    *  - giving free entity as a number (see Entity in Types.h);
    *  - destroying entity or just putting number back to the queue
    *  - assigning Signatures to the Entity (Signature is a list of Components that Entity has, it is represented as a bitset)
    *  - giving Signatures of the Entity
    */

    class EntityManager
    {
    public:
        static EntityManager& get();

        static Entity createEntity();
        static void destroyEntity(Entity entity);

        static void setSignature(Entity entity, Signature signature);
        static Signature getSignature(Entity entity);

    private:
        EntityManager();
        EntityManager(const EntityManager& other)   = delete;
        EntityManager(const EntityManager&& other)  = delete;

        std::queue<Entity>                  m_availableEntities;
        std::array<Signature, MAX_ENTITIES> m_signatures;
        uint32_t                            m_livingEntityCount;
    };
} // namespace Thomas
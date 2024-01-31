#pragma once

#include <bitset>
#include <cstdint>
#include <set>

namespace Thomas
{
    using Entity                        = std::uint32_t;
    const Entity MAX_ENTITIES           = 1000;

    using ComponentType                 = std::uint8_t;
    const ComponentType MAX_COMPONENTS  = 64;

    using Signature                     = std::bitset<MAX_COMPONENTS>;

    struct System
    {
        std::set<Entity> m_entities;

        virtual void onEntityAdded(Entity entity)   = 0;
        virtual void onEntityRemoved(Entity entity) = 0;
        virtual void onEntityDeleted(Entity entity) = 0;
    };
} // namespace Thomas
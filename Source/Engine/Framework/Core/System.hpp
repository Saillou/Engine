#pragma once

#include <set>

#include "ECSTypes.hpp"

struct System
{
    virtual ~System() = default;

    virtual void init() = 0; // Set signatures here
    virtual void addEntity(Entity entity);
    virtual void removeEntity(Entity entity);

    const std::set<Entity>& retrieve() const {
        return m_entities;
    }

protected:
    std::set<Entity> m_entities;
};
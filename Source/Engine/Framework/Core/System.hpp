#pragma once

#include <set>

#include "ECSTypes.hpp"

struct System
{
    virtual ~System() = default;

    virtual void addEntity(Entity entity);
    virtual void removeEntity(Entity entity);

protected:
    std::set<Entity> m_entities;
};
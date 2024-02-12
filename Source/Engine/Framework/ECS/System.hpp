#pragma once

#include <set>

#include "ECSTypes.hpp"

struct System
{
    virtual void addEntity(Entity entity)    {};
    virtual void removeEntity(Entity entity) {};

    std::set<Entity> m_entities;
};
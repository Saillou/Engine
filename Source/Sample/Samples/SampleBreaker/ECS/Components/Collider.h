#pragma once

#include <memory>
#include <Engine/Graphic/Base/Model/Entity.hpp>

#include "Sample/Samples/SampleBreaker/ECS/Core/Types.h"

struct ColliderComponent
{
    float searchDistance = 3.f;
    std::shared_ptr<Entity> collisionBox = nullptr;

    bool hasCollision = false;
    Thomas::Entity collidedEntity;
};
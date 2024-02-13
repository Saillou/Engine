#pragma once

#include <memory>
#include <Engine/Framework/Core/ECSTypes.hpp>
#include <Engine/Graphic/Base/Model/Model.hpp>

struct ColliderComponent
{
    float searchDistance = 3.f;
    Model::Ref collisionBox = nullptr;

    bool hasCollision = false;
    Entity collidedEntity;
};
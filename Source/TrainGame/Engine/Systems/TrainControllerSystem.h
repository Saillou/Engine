#pragma once

#include <Engine/Framework/ECS/System.hpp>

namespace Thomas
{
    struct TrainControllerSystem : public System
    {
        void init();
        void update(float dt);
    };
} // namespace Thomas
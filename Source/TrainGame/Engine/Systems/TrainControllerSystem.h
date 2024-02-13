#pragma once

#include <Engine/Framework/Core/System.hpp>

namespace Thomas
{
    struct TrainControllerSystem : public System
    {
        void init();
        void update(float dt);
    };
} // namespace Thomas
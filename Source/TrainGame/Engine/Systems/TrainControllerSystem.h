#pragma once

#include "TrainGame/Engine/Core/Types.h"

namespace Thomas
{
    class TrainControllerSystem : public System
    {
    public:
        void init();
        void update(float dt);

        void onEntityAdded(Entity entity) override {};
        void onEntityRemoved(Entity entity) override {};
        void onEntityDeleted(Entity entity) override {};
    };
} // namespace Thomas
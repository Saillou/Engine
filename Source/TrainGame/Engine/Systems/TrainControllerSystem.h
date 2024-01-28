#pragma once

#include "glm/glm.hpp"
#include "TrainGame/Engine/Core/Types.h"

namespace Thomas
{
    class TrainControllerSystem : public System
    {
    public:
        void update(float dt);

        void onEntityAdded(Entity entity) override {};
        void onEntityRemoved(Entity entity) override {};

    private:
        bool goToPoint(const glm::vec3& pos, float dt);
    };
} // namespace Thomas
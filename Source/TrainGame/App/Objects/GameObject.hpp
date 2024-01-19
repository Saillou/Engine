#pragma once

#include <vector>

#include "TrainGame/App/Objects/Transform.h"
#include "TrainGame/App/Objects/GameModel.hpp"

namespace Thomas
{
    class Component;

    struct GameObject
    {
        uint64_t    id; // unique for instance
        ModelId     modelId;
        Transform   transform; // world transform

        bool selectedForDeletion = false;

        std::vector<Component*> components;
    };
} // namespace Thomas
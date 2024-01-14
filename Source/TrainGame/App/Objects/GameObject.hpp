#pragma once

#include <vector>

#include "TrainGame/App/Objects/GameModel.hpp"

class Component;

struct GameObject
{
    uint64_t    id; // unique for instance
    ModelId     modelId;
    Transform   transform; // world transform

    std::vector<Component*> components;
};
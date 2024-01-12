#pragma once

#include <vector>

#include "Application/App/Objects/GameModel.hpp"

class Component;

struct GameObject
{
    uint64_t    id;
    ModelId     modelId;
    Transform   transform; // world transform

    std::vector<Component*> components;
};
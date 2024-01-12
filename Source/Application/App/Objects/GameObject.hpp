#pragma once

#include "GameModel.hpp"

struct GameObject
{
    uint64_t    id;
    ModelId     modelId;
    Transform   transform; // world transform
};
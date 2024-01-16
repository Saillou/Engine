#pragma once

#include "glm/glm.hpp"

#include "TrainGame/App/Objects/Transform.h"

namespace Thomas
{
    struct GridCell
    {
        enum class CellType
        {
            None = 0,
            Visible,
            ConstructOk,
            ConstructBad
        };

        Transform transform;
        CellType type = CellType::None;
    };
} // namespace Thomas
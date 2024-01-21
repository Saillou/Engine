#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Thomas
{
    struct Item
    {
        enum class ItemType
        {
            None = 0,
            Item1,
            Item2
        };

        ItemType type = ItemType::None;
    };
} // namespace Thomas
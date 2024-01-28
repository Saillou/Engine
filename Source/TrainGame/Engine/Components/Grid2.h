#pragma once

#include "glm/glm.hpp"

namespace Thomas
{
    struct Grid2
    {
        enum class GridComponentState
        {
            Invisible = 0,
            Visible,
            Construct
        };

        GridComponentState m_state;
        glm::vec2 m_size;
        glm::vec2 m_offset;
    };
} // namespace Thomas
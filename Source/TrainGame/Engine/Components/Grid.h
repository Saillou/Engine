#pragma once

#include <vector>
#include "glm/glm.hpp"

namespace Thomas
{
    struct Grid
    {
        enum class GridComponentState
        {
            Invisible = 0,
            Visible,
            Construct
        };

        GridComponentState  m_state;
        glm::ivec2          m_size{};
        glm::ivec2          m_offset{};
        std::vector<bool>   m_cells;

        void createCells()
        {
            m_cells.resize(m_size.x * m_size.y);

            for (size_t y = 0; y < m_size.y; ++y)
            {
                for (size_t x = 0; x < m_size.x; ++x)
                {
                    m_cells[x + (y * m_size.x)] = true;
                }
            }
        }
    };
} // namespace Thomas
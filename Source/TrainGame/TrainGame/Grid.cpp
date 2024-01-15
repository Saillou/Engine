#include "Grid.hpp"

namespace Thomas
{
    Grid& Grid::get()
    {
        static Grid grid;
        return grid;
    }

    void Grid::initAtPosition(const glm::vec3& pos, const glm::vec2& size)
    {
        Grid& grid = get();

        grid.m_initialised = true;
        grid.m_position = pos;
        grid.m_size = size;
    }
    glm::vec3 Grid::getPosition(int x, int y)
    {
        Grid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        return { 
            x * grid.m_size.x + grid.m_position.x,
            y * grid.m_size.y + grid.m_position.y,
            grid.m_position.z 
        };
    }
} // namespace Thomas
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
    std::pair<int64_t, int64_t> Grid::getPosition(const glm::vec3& pos)
    {
        Grid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        int64_t x = static_cast<int64_t>((pos.x + grid.m_position.x) / grid.m_size.x);
        int64_t y = static_cast<int64_t>((pos.y + grid.m_position.y) / grid.m_size.y);
        
        return {x,y};
    }

    void Grid::clearCells()
    {
        Grid& grid = get();

        assert(grid.m_initialised && "Grid is not initialised.");

        grid.m_cells.clear();
    }

    void Grid::setCellAtPosition(int x, int y, GridCell::CellType type)
    {
        Grid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        GridCell cell;
        cell.type = type;
        cell.transform.position = Grid::getPosition(x, y);
        cell.transform.scale = { 0.3f,0.3f, 0.01f };

        grid.m_cells[{x, y}] = cell;
    }

    const std::map<std::pair<int, int>, GridCell>& Grid::getCells()
    {
        Grid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        return grid.m_cells;
    }

} // namespace Thomas
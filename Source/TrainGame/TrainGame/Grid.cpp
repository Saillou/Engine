#include "Grid.hpp"

namespace Thomas
{
    OldGrid& OldGrid::get()
    {
        static OldGrid grid;
        return grid;
    }

    void OldGrid::initAtPosition(const glm::vec3& pos, const glm::vec2& size)
    {
        OldGrid& grid = get();

        grid.m_initialised = true;
        grid.m_position = pos;
        grid.m_size = size;
    }
    glm::vec3 OldGrid::getCellSize()
    {
        OldGrid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        return glm::vec3(grid.m_size, 0.1f);
    }
    glm::vec3 OldGrid::getPosition(int x, int y)
    {
        OldGrid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        return { 
            x * grid.m_size.x + grid.m_position.x,
            y * grid.m_size.y + grid.m_position.y,
            grid.m_position.z 
        };
    }
    std::pair<int64_t, int64_t> OldGrid::getPosition(const glm::vec3& pos)
    {
        OldGrid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        int64_t x = static_cast<int64_t>((pos.x + grid.m_position.x) / grid.m_size.x);
        int64_t y = static_cast<int64_t>((pos.y + grid.m_position.y) / grid.m_size.y);
        
        return {x,y};
    }

    glm::vec3 OldGrid::alignPosition(const glm::vec3& pos)
    {
        OldGrid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        // TODO: this is not the good way of doing it (probably)
        auto pair = getPosition(pos);
        return getPosition(pair.first, pair.second);
    }

    void OldGrid::clearCells()
    {
        OldGrid& grid = get();

        assert(grid.m_initialised && "Grid is not initialised.");

        grid.m_cells.clear();
    }

    void OldGrid::setCellAtPosition(int x, int y, GridCell::CellType type)
    {
        OldGrid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        GridCell cell;
        cell.type = type;
        //cell.transform.position = OldGrid::getPosition(x, y);
        //cell.transform.scale = 0.15f * glm::vec3{ 0.3f,0.3f, 0.01f };
        //cell.transform.rotation = { 0,0,0 };

        auto it = grid.m_cells.find({ x,y });

        if (it != grid.m_cells.end())
        {
            if (it->second.type == GridCell::CellType::ConstructBad)
                return;

            const bool cond1 = type == GridCell::CellType::ConstructOk && it->second.type == GridCell::CellType::Visible;
            const bool cond2 = type == GridCell::CellType::Visible && it->second.type == GridCell::CellType::ConstructOk;
            if (cond1 || cond2)
                cell.type = GridCell::CellType::ConstructBad;
        }

        grid.m_cells[{x, y}] = cell;
    }

    const std::map<std::pair<int, int>, GridCell>& OldGrid::getCells()
    {
        OldGrid& grid = get();

        assert(grid.m_initialised && "Grid is not initialized.");

        return grid.m_cells;
    }

} // namespace Thomas
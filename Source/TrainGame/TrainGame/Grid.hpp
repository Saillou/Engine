#pragma once

#include "glm/glm.hpp"
#include <map>
#include <utility>

#include "GridCell.hpp"

namespace Thomas
{
    class Grid
    {
    public:
        static Grid& get();

        static void initAtPosition(const glm::vec3& pos, const glm::vec2& size);

        static glm::vec3 getPosition(int x, int y);
        static std::pair<int64_t, int64_t> getPosition(const glm::vec3& pos);

        static void clearCells();
        static void setCellAtPosition(int x, int y, GridCell::CellType type);
        static const std::map<std::pair<int, int>, GridCell>& getCells();

    private:
        Grid() {};
        Grid(const Grid& other) = delete;
        Grid(const Grid&& other) = delete;

        bool m_initialised      = false;
        glm::vec3 m_position    = { 0,0,0 };
        glm::vec2 m_size        = { 0,0 };

        std::map<std::pair<int, int>, GridCell> m_cells;
    };
} // namespace Thomas
#pragma once

#include <map>
#include <glm/glm.hpp>
#include <Engine/Framework/Core/System.hpp>

namespace Thomas
{
    class GridSystem : public System
    {
    public:
        void init();
        void start(const glm::vec3& pos, const glm::vec2& size);
        void update(float dt);

    private:
        struct GridCell
        {
            enum class CellType
            {
                None = 0,
                Visible,
                ConstructOk,
                ConstructBad
            };

            Entity entity;
            CellType type = CellType::None;
        };

        glm::vec3 getPosition(int x, int y);
        std::pair<int, int> getPosition(const glm::vec3& pos);
        glm::vec3 alignPosition(const glm::vec3& pos);
        void setCellAtPosition(int x, int y, GridCell::CellType type);

        glm::vec3 m_position    = { 0,0,0 };
        glm::vec2 m_size        = { 0,0 };
        std::map<std::pair<int,int>, GridCell> m_cells{};
    };
} // namespace Thomas
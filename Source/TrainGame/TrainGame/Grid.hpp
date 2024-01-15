#pragma once

#include "glm/glm.hpp"

namespace Thomas
{
    class Grid
    {
    public:
        static Grid& get();

        static void initAtPosition(const glm::vec3& pos, const glm::vec2& size);
        static glm::vec3 getPosition(int x, int y);

    private:
        Grid() {};
        Grid(const Grid& other) = delete;
        Grid(const Grid&& other) = delete;

        bool m_initialised      = false;
        glm::vec3 m_position    = { 0,0,0 };
        glm::vec2 m_size        = { 0,0 };
    };
} // namespace Thomas
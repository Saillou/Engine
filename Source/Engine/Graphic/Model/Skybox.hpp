#pragma once

#include <string>
#include <array>

#include "../Base/Shapes/Box.hpp"
#include "../Utils/Array.hpp"
#include "../Utils/Buffer.hpp"
#include "../Utils/Texture.hpp"

struct Skybox {
    Skybox(const std::array<std::string, 6>& textures_path);

    void draw(const Camera& camera);

private:
    Shader shader;

    Array m_vao;
    Buffer m_vbo;
    Texture m_texture;

    static const std::vector<float> _getCubeVertices();
};

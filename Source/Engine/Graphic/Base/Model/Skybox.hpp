#pragma once

#include <string>
#include <array>

#include "../../Camera.hpp"
#include "../Model/Primitive/Cube.hpp"

struct Skybox {
    Skybox(const std::array<std::string, 6>& textures_path);

    void draw(const Camera& camera);

private:
    Shader& shader;

    Array m_vao;
    Buffer m_vbo;
    Texture m_texture;

    static const std::vector<float> _getCubeVertices();
};

#pragma once

#include <string>
#include <array>
#include "../Base/Shapes/Box.hpp"

struct Skybox {
    Skybox(const std::array<std::string, 6>& textures_path);
    ~Skybox();

    void draw(const Camera& camera);

private:
    Shader shader;
    std::vector<float> skyboxVertices;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int texture = 0;
};

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Utils/Shader.hpp"
#include "../../Utils/Array.hpp"
#include "../../Utils/Buffer.hpp"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TextureData {
    unsigned int id = 0;
    std::string type;
};

class Mesh {
    friend class Model;

public:
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures);
    void draw(Shader& shader);
    void drawElements();

private:
    Array m_vao;
    Buffer m_vbo;
    Buffer m_ebo;

    std::vector<Vertex>       vertices = {};
    std::vector<unsigned int> indices  = {};
    std::vector<TextureData>  textures = {};

    void _setup();
};
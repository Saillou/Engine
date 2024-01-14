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
    void draw(Shader& shader, const glm::mat4& quat = glm::mat4(1.0f));
    void drawElements(Shader& shader, const glm::mat4& quat = glm::mat4(1.0f));

    const std::vector<Vertex>&       vertices() const;
    const std::vector<unsigned int>& indices() const;

private:
    Array m_vao;
    Buffer m_vbo;
    Buffer m_ebo;

    std::vector<Vertex>       m_vertices = {};
    std::vector<unsigned int> m_indices  = {};
    std::vector<TextureData>  m_textures = {};

    void _setup();
};
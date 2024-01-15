#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Utils/Shader.hpp"
#include "../../Utils/Array.hpp"
#include "../../Utils/Buffer.hpp"
#include "../../Utils/Texture.hpp"

#include <string>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TextureData {
    std::string type;
    std::unique_ptr<Texture> data;
};

struct Mesh {
    friend struct Model;
    friend struct Cube;
    friend struct Sphere;

    Mesh();

    void bindTextures(Shader& shader);
    void unbindTextures();
    void drawElements(Shader& shader, const glm::mat4& quat = glm::mat4(1.0f));

    const std::vector<Vertex>&       vertices() const;
    const std::vector<unsigned int>& indices() const;

private:
    Array  m_vao;
    Buffer m_vbo;
    Buffer m_ebo;

    std::vector<Vertex>       m_vertices = {};
    std::vector<unsigned int> m_indices  = {};
    std::vector<TextureData>  m_textures = {};

    void _setup();
};
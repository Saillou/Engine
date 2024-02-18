#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../Wrapper/Shader.hpp"
#include "../../Wrapper/Array.hpp"
#include "../../Wrapper/Buffer.hpp"
#include "../../Wrapper/Texture.hpp"

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
    Mesh();
    void setupVao();
    void compute_obb();

    void bindTextures(Shader& shader) const;
    void unbindTextures() const;

    void drawElements(size_t nElements = 1) const;

    // Cpu data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureData>  textures;

    const glm::mat4& obb() const;

    // Gpu instructions
    enum DrawMode : unsigned int
    {
        Point = GL_POINTS,
        Line = GL_LINES,
        Triangle = GL_TRIANGLES,
    } drawMode = Triangle;

private:
    // Gpu data
    Array  m_vao;
    Buffer m_vbo;
    Buffer m_ebo;

    // Cached computations
    glm::mat4 m_obb = glm::mat4(1.0f);
};

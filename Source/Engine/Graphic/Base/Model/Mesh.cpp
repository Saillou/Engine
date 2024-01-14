#include "Mesh.hpp"


Mesh::Mesh() :
    m_vbo(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER)
{
}

// render the mesh
void Mesh::bindTextures(Shader& shader) {
    // Bind textures
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        if(!shader.has(m_textures[i].type))
            continue;

        Texture::activate(GL_TEXTURE0 + i);
        shader.set(m_textures[i].type, (int)i);
        m_textures[i].data->bind();
    }
}

void Mesh::drawElements(Shader& shader, const glm::mat4& quat) {
    shader.use().set("LocalModel", quat);

    m_vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}

const std::vector<Vertex>& Mesh::vertices() const {
    return m_vertices;
}

const std::vector<unsigned int>& Mesh::indices() const {
    return m_indices;
}

// initializes all the buffer objects/arrays
void Mesh::_setup() {
    m_vao.bind();
    m_vbo.bindData(m_vertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    m_ebo.bindData(m_indices);
    m_vao.unbind();
}

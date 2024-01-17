#include "Mesh.hpp"

Mesh::Mesh():
    m_vbo(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER), 
    m_colors(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
    m_instances(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
}

void Mesh::createBatch(size_t size) {
    m_vao.bind();

    m_colors.bindData(size * sizeof(glm::vec4));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glVertexAttribDivisor(3, 1);

    m_instances.bindData(size * sizeof(glm::mat4));
    for (int i = 0; i < 4; i++) {
        glEnableVertexAttribArray(4 + i);
        glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(4 + i, 1);
    }

    m_vao.unbind();
}

void Mesh::updateBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models) {
    m_colors.bindData(colors);
    m_instances.bindData(models);
}

void Mesh::drawElementsBatch() {
    m_vao.bind();
    glDrawElementsInstanced(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0, (GLsizei)m_instances.size() / sizeof(glm::mat4));
    m_vao.unbind();
}

// render the mesh
void Mesh::bindTextures(Shader& shader) {
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        if(!shader.has(m_textures[i].type))
            continue;

        Texture::activate(GL_TEXTURE0 + i);
        shader.set(m_textures[i].type, (int)i);
        m_textures[i].data->bind();
    }
}

void Mesh::unbindTextures() {
    for (unsigned int i = 0; i < m_textures.size(); i++) {
        m_textures[i].data->unbind();
    }
}

void Mesh::drawElements() {
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, (unsigned int)m_indices.size(), GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}

void Mesh::drawElements(Shader& shader, const glm::mat4& quat) {
    if(shader.has("LocalModel"))
        shader.use().set("LocalModel", quat);

    drawElements();
}

// Getters
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

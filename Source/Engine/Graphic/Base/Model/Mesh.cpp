#include "Mesh.hpp"


Mesh::Mesh() :
    m_vbo(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER)
{
}

Mesh::Mesh(std::vector<Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<TextureData> textures_) :
    m_vbo(GL_ARRAY_BUFFER),
    m_ebo(GL_ELEMENT_ARRAY_BUFFER),

    vertices(vertices_),
    indices(indices_),
    textures(textures_)
{
    _setup();
}

// render the mesh
void Mesh::draw(Shader& shader) {
    // Bind textures
    unsigned int diffuseNr = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);

        glUniform1i(glGetUniformLocation(shader.getId(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    drawElements();

    // Unbind textures
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::drawElements() {
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    m_vao.unbind();
}

// initializes all the buffer objects/arrays
void Mesh::_setup() {
    m_vao.bind();
    m_vbo.bindData(vertices);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    m_ebo.bindData(indices);
    m_vao.unbind();
}

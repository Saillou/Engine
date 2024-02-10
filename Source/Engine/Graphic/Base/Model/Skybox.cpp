#include "Skybox.hpp"

#include <iostream>
#include <stb_image.h>

Skybox::Skybox(const std::array<std::string, 6>& textures_path):
    m_vbo(GL_ARRAY_BUFFER),
    m_texture(GL_RGB, GL_TEXTURE_CUBE_MAP)
{
    // Vertices
    m_vao.bind();
    m_vbo.bindData(_getCubeVertices());

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Textures
    for (unsigned int i = 0; i < textures_path.size(); i++) {
        m_texture.load(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textures_path[i].c_str());
    }

    // Shader
    shader.
        attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")

            .add_var("uniform", "mat4", "view")
            .add_var("uniform", "mat4", "projection")

            .add_var("out", "vec3", "TexCoords")

            .add_func("void", "main", "", R"_main_(
                TexCoords = aPos;
                vec4 pos = projection * view * vec4(aPos, 1.0);
                gl_Position = pos.xyww;
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("in", "vec3", "TexCoords")
            .add_var("uniform", "samplerCube", "skybox")
            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = texture(skybox, TexCoords);
            )_main_")
        )
        .link();

    shader.use();
    shader.set("skybox", 0);
}

void Skybox::draw(const Camera& camera) {
    // depth test shall pass when values are equal to depth buffer's content
    glDepthFunc(GL_LEQUAL);

    shader.use();
    shader.set("view", glm::mat4(glm::mat3(camera.modelview))); // remove translation from the view matrix
    shader.set("projection", camera.projection);

    // skybox cube
    m_vao.bind();
    {
        m_texture.activate(GL_TEXTURE0);
        m_texture.bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    m_vao.unbind();

    // Original value
    glDepthFunc(GL_LESS);
}

const std::vector<float> Skybox::_getCubeVertices() {
    return std::vector<float> {      
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
}

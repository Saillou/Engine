#include "ShadowRender.hpp"

#include <glad/glad.h>

ShadowRender::ShadowRender()
{
    shader.
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")
            .add_var("layout (location = 3) in", "vec4", "aColor")
            .add_var("layout (location = 4) in", "mat4", "aModel")

            .add_var("uniform", "mat4", "LocalModel")
            .add_func("void", "main", "", R"_main_(
                gl_Position = aModel * LocalModel * vec4(aPos, 1.0);
            )_main_")
            .str()
        ).
        attachSource(GL_GEOMETRY_SHADER, ShaderSource{}
            .add_var("in", "layout", "(triangles)")
            .add_var("uniform", "mat4", "shadowMatrices[6]")
            .add_var("out", "layout", "(triangle_strip, max_vertices = 18)")
            .add_var("out", "vec4", "FragPos")
            .add_func("void", "main", "", R"_main_(
                for(int face = 0; face < 6; face++) {
                    gl_Layer = face;                    // built-in variable: face rendered
                    for(int i = 0; i < 3; ++i) {
                        FragPos = gl_in[i].gl_Position;
                        gl_Position = shadowMatrices[face] * FragPos;
                        EmitVertex();
                    }    
                    EndPrimitive();
                }
            )_main_")
            .str()
        ).
        attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec4", "FragPos")
            .add_var("uniform", "vec3", "lightPos")
            .add_var("uniform", "float", "far_plane")
            .add_func("void", "main", "", R"_main_(
                // map to [0;1] range by dividing by far_plane as modified depth
                gl_FragDepth = length(FragPos.xyz - lightPos) / far_plane; 
            )_main_")
            .str()
        ).
        link();
}

void ShadowRender::bindTextures(GLuint target) const {
    for (int i = 0; i < (int)fbos.size(); i++) {
        Texture::activate(target + i);
        fbos[i]->texture().bind();
    }
}

int ShadowRender::width() const {
    return 1024;
}
int ShadowRender::height() const {
    return 1024;
}

void ShadowRender::render(const Camera& camera, const std::vector<Light>& lights, std::function<void(Shader&)> renderFunc) 
{
    if (fbos.size() > lights.size())
        fbos.resize(lights.size());

    for (int i = 0; i < (int)lights.size(); i++) 
    {
        if (fbos.size() < lights.size())
            fbos.push_back(std::make_unique<Framebuffer>(Framebuffer::Cubemap, width(), height(), GL_DEPTH_ATTACHMENT));

        fbos[i]->bind();
        fbos[i]->clear();

        const Light& light = lights[i];
        const glm::vec3& lightPos = light.position;

        // Create depth cubemap transformation matrices
        const glm::mat4 shadowProj = glm::perspective(1.57f, 1.0f, camera.near_plane, camera.far_plane);
        const std::array<glm::mat4, 6> shadowTransforms =
        {
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(+1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, +1.0f,  0.0f), glm::vec3(0.0f,  0.0f, +1.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,  0.0f, +1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        // Render scene to depth cubemap
        {
            shader.use();
            for (unsigned int i = 0; i < 6; i++) {
                shader.set("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            }
            shader.set("far_plane", camera.far_plane);
            shader.set("lightPos", lightPos);

            renderFunc(shader);
        }

        fbos[i]->unbind();
    }
}

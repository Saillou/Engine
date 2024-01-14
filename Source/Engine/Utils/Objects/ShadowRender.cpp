#include "ShadowRender.hpp"

#include <glad/glad.h>

ShadowRender::ShadowRender() : 
	fbo(Framebuffer::Cubemap, 1024, 1024, GL_DEPTH_ATTACHMENT)
{
    shader.
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("uniform", "mat4", "model")
            .add_var("uniform", "mat4", "LocalModel")
            .add_func("void", "main", "", R"_main_(
                gl_Position = model * LocalModel * vec4(aPos, 1.0);
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

void ShadowRender::bindTexture(GLuint target) {
    Texture::activate(target);
    fbo.texture().bind();
}

int ShadowRender::width() const {
    return fbo.width();
}
int ShadowRender::height() const {
    return fbo.height();
}

void ShadowRender::render(const Camera& camera, const Light& light, std::function<void(Shader&)> renderFunc) 
{
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
    fbo.bind();
    fbo.clear();
    {
        shader.use();
        for (unsigned int i = 0; i < 6; i++) {
            shader.set("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
        }
        shader.set("far_plane", camera.far_plane);
        shader.set("lightPos", lightPos);

        renderFunc(shader);
    }
    fbo.unbind();
}

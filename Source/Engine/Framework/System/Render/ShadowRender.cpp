#include "ShadowRender.hpp"
#include "../../../Graphic/ShaderManager.hpp"

#include <glad/glad.h>

ShadowRender::ShadowRender():
    shader(ShaderManager::Get(CookType::Shadow))
{ }

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

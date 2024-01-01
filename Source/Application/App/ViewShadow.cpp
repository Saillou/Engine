#include "ViewShadow.hpp"

ViewShadow::ViewShadow(int widthHint, int heightHint) :
    BaseScene(widthHint, heightHint)
{
    // World
    m_camera.position  = glm::vec3(5.0f, -12.0f, 5.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0f, 0.0f);
    m_lights = { 
        Light(glm::vec3{ 0,0,0 }, glm::vec4{ 1,1,1,1 }) 
    };

    // Objects
    box = std::make_unique<Box>(2.0f);
    box->addRecipe(Cookable::CookType::Shadow);
}

void ViewShadow::draw() {
    // World is moving
    _update_camera();

    // Render scene in shadow mode
    m_shadowRender.render(m_camera, m_lights[0], [=](Shader& sh) {
        renderScene(sh);
    });

    // Render scene as normal
    BaseScene::Viewport(m_width, m_height);
    BaseScene::clear();
    {
        box->get(Cookable::CookType::Shadow)->use()
            .set("projection",  m_camera.projection)
            .set("view",        m_camera.modelview)
            .set("viewPos",     m_camera.position)
            .set("far_plane",   m_camera.far_plane)
            .set("lightPos",    m_lights[0].position)
            .set("depthMap",    0);

        Texture::activate(GL_TEXTURE0);
        m_shadowRender.bindTexture();

        renderScene(*box->get(Cookable::CookType::Shadow));
    }

    m_time += 1.0f / 60.0f;
}

void ViewShadow::renderScene(Shader& shader)
{
    renderGround(shader);
    renderCube(shader);
}

void ViewShadow::renderGround(Shader& shader) {
    if (shader.has("diffuseColor"))
        shader.use().set("diffuseColor", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

    constexpr int n_cells = 15;
    constexpr float size = 0.30f;

    for (int x = -n_cells; x < n_cells; x++) {
        for (int y = -n_cells; y < n_cells; y++) {
            shader.use()
                .set("model",
                glm::scale(
                    glm::translate(
                        glm::mat4(1.0f),                // identity
                        size * glm::vec3(x, y, -3.0f)), // translation
                    glm::vec3(size))                    // scale
            );

            box->bind();
            glDrawElements(GL_TRIANGLES, (int)box->indicesLength(), GL_UNSIGNED_INT, 0);
            box->unbind();
        }
    }
}

void ViewShadow::renderCube(Shader& shader) {
    if (shader.has("diffuseColor"))
        shader.use().set("diffuseColor", glm::vec4(1.0f, 0.7f, 0.3f, 1.0f));

    shader.use()
        .set("model",
        glm::scale(
            glm::translate(
                glm::mat4(1.0f),                // identity
                glm::vec3(-1.5f, 2.0f, +0.0f)), // translation
            glm::vec3(0.75f))                   // scale
    );

    box->bind();
    glDrawElements(GL_TRIANGLES, (int)box->indicesLength(), GL_UNSIGNED_INT, 0);
    box->unbind();
}

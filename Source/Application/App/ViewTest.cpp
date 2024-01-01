#include "ViewTest.hpp"

ViewTest::ViewTest(int widthHint, int heightHint) :
    BaseScene(widthHint, heightHint)
{
    // World
    m_camera.position  = glm::vec3(5.0f, -4.5f, 3.0f);
    m_camera.direction = glm::vec3(0.0f, 0.0f, 0.0f);
    m_lights = { 
        Light(glm::vec3{ 0,0,0 }, glm::vec4{ 1,1,1,1 }) 
    };

    // Resources
    textureWood = std::make_unique<Texture>("Resources/textures/wood.png");

    // Objects
    box = std::make_unique<Box>(2.0f);
    box->addRecipe(Cookable::CookType::Shadow);

    // Configuration
    box->addRecipe(Cookable::CookType::Shadow);
    box->get(Cookable::CookType::Shadow)->use()
        .set("diffuseTexture", 0)
        .set("depthMap", 1);
}

void ViewTest::draw() {
    // World is moving
    _update_camera();
    m_lights[0].position.z = std::sin(m_time * 0.5f) * 3.0f;

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
            .set("lightPos",    m_lights[0].position)
            .set("viewPos",     m_camera.position)
            .set("far_plane",   m_camera.far_plane);

        Texture::activate(GL_TEXTURE0);
        textureWood->bind();

        Texture::activate(GL_TEXTURE1);
        m_shadowRender.bindTexture();

        renderScene(*box->get(Cookable::CookType::Shadow));
    }

    m_time += 1.0f / 60.0f;
}

void ViewTest::renderScene(Shader& shader)
{
    // room cube
    shader.use().set("model", glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));

    // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    glDisable(GL_CULL_FACE);
    {
        if (shader.has("reverse_normals"))
            shader.set("reverse_normals", true); // A small little hack to invert normals when drawing cube from the inside so lighting still works.

        renderCube();

        if (shader.has("reverse_normals"))
            shader.set("reverse_normals", false); // and of course disable it
    }
    glEnable(GL_CULL_FACE);
    
    // cube
    shader.set("model",
        glm::scale(
            glm::rotate(
                glm::translate(
                    glm::mat4(1.0f),                                        // identity
                glm::vec3(-1.5f, 2.0f, -3.0)),                              // translation
            glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))), // rotation
        glm::vec3(0.75f))                                                   // scale
    );

    renderCube();
}

void ViewTest::renderCube() {
    box->bind();
    glDrawElements(GL_TRIANGLES, (int)box->indicesLength(), GL_UNSIGNED_INT, 0);
    box->unbind();
}

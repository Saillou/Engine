    #include "Box.hpp"

Box::Box(float size) 
{
    _setupMesh(0.5f * size * glm::vec3(1, 1, 1));
}

Box::Box(const glm::vec3& dims)
{
    _setupMesh(dims);
}

void Box::_setupMesh(const glm::vec3& dims) {
    using v3 = glm::vec3;
    using v2 = glm::vec2;

    const glm::mat3 world(1.0f); // identity

    // Space dimensions
    const v3 u = dims[0] * world[0];
    const v3 v = dims[1] * world[1];
    const v3 w = dims[2] * world[2];

    // Vertices positions
    const v3 pA = -u + v - w;
    const v3 pB = +u + v - w;
    const v3 pC = +u - v - w;
    const v3 pD = -u - v - w;

    const v3 pE = -u + v + w;
    const v3 pF = +u + v + w;
    const v3 pG = +u - v + w;
    const v3 pH = -u - v + w;

    // Texture coordinates
    const v2 tA = glm::vec2(0.0f, 0.0f);
    const v2 tB = glm::vec2(1.0f, 0.0f);
    const v2 tC = glm::vec2(0.0f, 1.0f);
    const v2 tD = glm::vec2(1.0f, 1.0f);


    _createQuad(pD, pC, pB, pA, -w, tA, tB, tC, tD);
    _createQuad(pH, pG, pF, pE, +w, tA, tB, tC, tD);

    _createQuad(pE, pA, pD, pH, -u, tA, tB, tC, tD);
    _createQuad(pF, pB, pC, pG, +u, tA, tB, tC, tD);

    _createQuad(pD, pC, pG, pH, -v, tA, tB, tC, tD);
    _createQuad(pA, pB, pF, pE, +v, tA, tB, tC, tD);

    // Bind
    _bindArray();
}

void Box::draw(const Camera& camera, const glm::mat4& model, const std::vector<Light>& lights) {
    for (auto& recipe : m_shaders) {
        // Setup uniforms
        recipe.second->
            use().
            set("Model", model).
            set("View", camera.modelview).
            set("Projection", camera.projection);

        switch (recipe.first)
        {
        case CookType::Solid:
            recipe.second->
                set("CameraPos", camera.position).
                set("LightNumber", (int)lights.size());

            for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
                recipe.second->
                    set("LightPos_" + std::to_string(iLight), lights[iLight].position).
                    set("LightColor_" + std::to_string(iLight), lights[iLight].color);
            }
            break;

        case CookType::Shadow:
            recipe.second->
                set("viewPos", camera.position).
                set("far_plane", camera.far_plane).
                set("lightPos", lights[0].position).
                set("lightColor", 0.3f*lights[0].color).
                set("depthMap", 1);

            break;
        }

        // Draw
        drawElements();
    }
}

void Box::draw(const Camera& camera, const glm::vec3& position, const glm::vec3& orientation, const std::vector<Light>& lights) {
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, orientation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, orientation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, orientation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    draw(camera, model, lights);
}

void Box::drawBatch(size_t amount, const Camera& camera) {
    for (auto& recipe : m_shaders) {
        // Setup uniforms
        recipe.second->
            use().
            set("View",         camera.modelview).
            set("Projection",   camera.projection).
            set("CameraPos",    camera.position);

        // Draw
        bind();
        glDrawElementsInstanced(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0, (GLsizei)amount);
        unbind();
    }
}

void Box::drawElements() {
    bind();
    glDrawElements(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);
    unbind();
}

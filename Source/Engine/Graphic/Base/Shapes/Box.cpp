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

    const glm::mat3 world(1.0f); // identity

    const v3 u = dims[0] * world[0];
    const v3 v = dims[1] * world[1];
    const v3 w = dims[2] * world[2];

    // Vertices positions
    const v3 A = -u + v - w;
    const v3 B = +u + v - w;
    const v3 C = +u - v - w;
    const v3 D = -u - v - w;

    const v3 E = -u + v + w;
    const v3 F = +u + v + w;
    const v3 G = +u - v + w;
    const v3 H = -u - v + w;

    _createQuad(D, C, B, A, -w);
    _createQuad(H, G, F, E, +w);

    _createQuad(E, A, D, H, -u);
    _createQuad(F, B, C, G, +u);

    _createQuad(D, C, G, H, -v);
    _createQuad(A, B, F, E, +v);

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

        if (recipe.first != Cookable::CookType::Geometry)
        {
            recipe.second->
                use().
                set("CameraPos", camera.position).
                set("LightNumber", (int)lights.size());

            for (int iLight = 0; iLight < (int)lights.size(); iLight++) {
                recipe.second->
                    set("LightPos_" + std::to_string(iLight), lights[iLight].position).
                    set("LightColor_" + std::to_string(iLight), lights[iLight].color);
            }
        }

        // Draw
        bind();
        glDrawElements(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);
        unbind();
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

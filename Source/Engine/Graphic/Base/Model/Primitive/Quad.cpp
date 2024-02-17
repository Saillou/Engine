#include "Quad.hpp"

#include "../../../Cookable.hpp"

std::unique_ptr<Mesh> Quad::CreateMesh(bool sendToGpu)
{
    std::unique_ptr<Mesh> quad_mesh = std::make_unique<Mesh>();

    // Shape
    createQuad(*quad_mesh,
        glm::vec3(-1, +1, 0), glm::vec3(-1, -1, 0), glm::vec3(+1, -1, 0), glm::vec3(+1, +1, 0),
        glm::vec3(0, 0, 1),
        glm::vec2(0, 0), glm::vec2(0, 1), glm::vec2(1, 1), glm::vec2(1, 0)
    );

    // Send to gpu
    if (sendToGpu)
        quad_mesh->sendToGpu();

    quad_mesh->compute_obb();
    return quad_mesh;
}

// - Quad -
Quad::Quad(float x_, float y_, float w_, float h_):
    x(x_), y(y_), w(w_), h(h_)
{
    _quad_mesh = CreateMesh();
}

void Quad::drawElements() const {
    _quad_mesh->drawElements();
}

glm::mat4 Quad::transform() const
{
    return 
        glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3(2.0f * x + (w - 1.0f), -y * 2.0f + (1.0f - h), 0.0f)), 
            glm::vec3(w, h, 0.0f)
        );
}

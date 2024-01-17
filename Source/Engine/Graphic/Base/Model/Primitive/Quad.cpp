#include "Quad.hpp"

#include "../../../Cookable.hpp"

// - Quad -
Quad::Quad(float x, float y, float w, float h)
{
    Cookable::Set(Cookable::CookType::Quad, shader_quad);

    float x1 = x*2.0f - 1.0f;
    float y1 = y*2.0f - 1.0f;

    float x2 = (x+w)*2.0f - 1.0f;
    float y2 = (y+h)*2.0f - 1.0f;

    // Shape
    createQuad(quad_mesh,
        glm::vec3(x1, y2, 0),
        glm::vec3(x1, y1, 0),
        glm::vec3(x2, y1, 0),
        glm::vec3(x2, y2, 0)
    );

    setupGPU(quad_mesh);
}

void Quad::draw() {
    shader_quad.use();
    drawElements();
}

void Quad::drawElements() {
    quad_mesh.drawElements();
}

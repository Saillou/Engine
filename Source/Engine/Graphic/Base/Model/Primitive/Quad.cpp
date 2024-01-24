#include "Quad.hpp"

#include "../../../Cookable.hpp"

// - Quad -
Quad::Quad(float x, float y, float w, float h):
    _x(x), _y(y), _w(w), _h(h)
{
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

void Quad::drawElements() const {
    quad_mesh.drawElements();
}

float Quad::x() const {
    return _x;
}
float Quad::y() const {
    return _y;
}
float Quad::w() const {
    return _w;
}
float Quad::h() const  {
    return _h;
}
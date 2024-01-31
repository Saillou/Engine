#include "Quad.hpp"

#include "../../../Cookable.hpp"

// - Quad -
Quad::Quad(float x, float y, float w, float h):
    _x(x), _y(y), _w(w), _h(h)
{
    // Shape
    createQuad(quad_mesh,
        glm::vec3(-1, +1, 0),
        glm::vec3(-1, -1, 0),
        glm::vec3(+1, -1, 0),
        glm::vec3(+1, +1, 0)
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

float& Quad::x() {
    return _x;
}
float& Quad::y() {
    return _y;
}
float& Quad::w() {
    return _w;
}
float& Quad::h() {
    return _h;
}

Pose Quad::pose() const {
    return Pose(glm::vec3(2.0f*_x + (_w - 1.0f), -_y*2.0f + (1.0f - _h), 0.0f), glm::vec3(_w, _h, 0.0f));
}

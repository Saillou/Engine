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
        setupGPU(*quad_mesh);

    return quad_mesh;
}

// - Quad -
Quad::Quad(float x, float y, float w, float h):
    _x(x), _y(y), _w(w), _h(h)
{
    _quad_mesh = CreateMesh();
}

void Quad::drawElements() const {
    _quad_mesh->drawElements();
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
    return Pose
    (
        glm::vec3(2.0f*_x + (_w - 1.0f), -_y*2.0f + (1.0f - _h), 0.0f), 
        glm::vec3(_w, _h, 0.0f)
    );
}

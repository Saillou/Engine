#include "Button.hpp"

Button::Button(const std::string& text):
    Widget(Tag::Button, EventListened::MouseMove | EventListened::MouseButton),
    text(text),
    backgroundColor(0.7f, 0.5f, 0.3f, 1.0f),
    foregroundColor(1,1,1,1)
{
    glm::vec2 sizeMini = TextEngine::MeasureRel(text, 0.5f);
    float padding = 0.05f;

    // Only 1 quad
    _surfaces.emplace_back(
        std::make_unique<Quad>(0.0f, 0.0f, sizeMini.x + padding, sizeMini.y + padding)
    );
}

void Button::draw(Scene& scene) {
    auto& surface = _surfaces.front();

    // Update data
    surface->x() = x;
    surface->y() = y;
    surface->material.diffuse_color = isMouseOver() ? foregroundColor : backgroundColor;
    
    // Draw
    scene.renderer().quad(*surface);

    glm::vec2 size = TextEngine::Measure(text, 0.5f);
    scene.renderer().text(text,
        scene.width()  * (surface->x() + surface->w()/2.0f) - size.x/2.0f,
        scene.height() * (1.0f-surface->y() - surface->h()/2.0f) - size.y/2.0f,
        0.5f,
        isMouseOver() ? backgroundColor : foregroundColor
    );
}

float& Button::w() {
    return _surfaces.front()->w();
}

float& Button::h() {
    return _surfaces.front()->h();
}


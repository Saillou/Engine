#include "Button.hpp"

Button::Button(const std::string& text_):
    Widget(Tag::Button, EventListened::MouseMove | EventListened::MouseButton),
    content(text_),
    backgroundColor(0.7f, 0.5f, 0.3f, 1.0f),
    foregroundColor(1,1,1,1)
{
    glm::vec2 sizeMini = content.getSizeRel();
    float padding = 0.05f;

    w() = sizeMini.x + padding;
    h() = sizeMini.y + padding;
}

void Button::draw(Scene& scene) {
    // Update data
    _surface->material.diffuse_color = isMouseOver() ? foregroundColor : backgroundColor;
    content.foregroundColor = isMouseOver() ? backgroundColor : foregroundColor;
    
    // Centered text
    glm::vec2 s = content.getSizeRel();
    content.x() = x() + (w() - s.x) / 2.0f;
    content.y() = y() + (h() + s.y) / 2.0f;

    // Draw
    scene.renderer().quad(*_surface);
    content.draw(scene);
}

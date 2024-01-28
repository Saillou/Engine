#include "Button.hpp"

Button::Button(const std::string& text_):
    Widget(Style::Tag::Button, EventListened::MouseMove | EventListened::MouseButton),
    content(text_)
{
    // Style
    style().background()   = glm::vec4(0.7f, 0.5f, 0.3f, 1.0f);
    style().contentAlign() = {Style::Align::Centered, Style::Align::Centered};

    // Dimensions
    glm::vec2 sizeMini = content.getSize();
    float padding = 0.05f;

    w() = sizeMini.x + padding;
    h() = sizeMini.y + padding;
}

void Button::draw(Scene& scene) {
    // Get style
    Widget::_applyStyle();

    _surface->material.diffuse_color = isMouseOver() ? style().foreground() : style().background();
    content.style().set_foreground(    isMouseOver() ? style().background() : style().foreground());
    
    // Centered text
    content.x() = x();
    content.y() = y();

    if(style().contentAlign().h == Style::Align::Centered)
        content.x() += (w() - content.w()) / 2.0f;
    
    if (style().contentAlign().v == Style::Align::Centered)
        content.y() += (h() + content.h()) / 2.0f;

    // Draw
    scene.renderer().quad(*_surface);
    content.draw(scene);
}

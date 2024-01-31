#include "Button.hpp"

Button::Button(const std::string& text_):
    Widget(Style::Tag::Button, EventListened::MouseMove | EventListened::MouseButton),
    content(text_)
{
    // Dimensions
    glm::vec2 sizeMini = content.getSize();

    w() = sizeMini.x + padding;
    h() = sizeMini.y + padding;
}

void Button::draw(Scene& scene) {
    // Get style
    Widget::_applyStyle();

    _surface->material.diffuse_color = isMouseOver() ? style().foreground() : style().background();
    content.style().set_foreground(    isMouseOver() ? style().background() : style().foreground());

    // Propagate style to text
    content.style().set_contentAlign(style().contentAlign());
    
    // Left align text
    content.x() = x();
    content.y() = y();

    // Centered
    if(style().contentAlign().h == Style::Align::Centered)
        content.x() += (w() - content.w()) / 2.0f;
    
    if (style().contentAlign().v == Style::Align::Centered)
        content.y() += (h() - content.h()) / 2.0f;

    // Draw
    scene.renderer().quad(*_surface);
    content.draw(scene);
}

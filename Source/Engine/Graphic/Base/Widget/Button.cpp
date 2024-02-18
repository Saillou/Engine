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

    if (isMouseClicked())
    {
        _surface->diffuse_color = style().clicked_background();
        content.style().foreground.setValue(style().clicked_foreground());
    }
    else
    {
        _surface->diffuse_color = isMouseOver() ? style().hovered_background() : style().background();
        content.style().foreground.setValue(isMouseOver() ? style().hovered_foreground() : style().foreground());
    }

    // Propagate style to text
    content.style().contentAlign.setValue(style().contentAlign());
    
    // Left align text
    content.x() = x();
    content.y() = y();

    // Centered
    if(style().contentAlign().h == Style::Align::Centered)
        content.x() += (w() - content.w()) / 2.0f;
    
    if (style().contentAlign().v == Style::Align::Centered)
        content.y() += (h() - content.h()) / 2.0f;

    // Draw
    //scene.renderer().quad(*_surface); TODO: Don't forget me
    content.draw(scene);
}

#include "VerticalLayout.hpp"

VerticalLayout::VerticalLayout(Scene& scene, Layout::ContainerType extType) :
    Layout(scene),
    m_extType(extType)
{
    // ...
}

void VerticalLayout::draw(Scene& scene) {
    float w0 = _parent ? _parent->w() : 1.0f;
    float h0 = _parent ? _parent->h() : 1.0f;

    // Position
    float yi = y() + (h0 - h()) / 2.0f;

    for (auto& widget : m_widgets) {
        widget->x() = x() + (w0 - widget->w())/2.0f;
        widget->y() = yi;
        widget->draw(m_scene);

        yi += widget->h();
    }
}

glm::vec2 VerticalLayout::_compute_hull_size() {
    if (m_widgets.empty())
        return glm::vec2(0, 0);

    // It's a vertical layout, hull: [w:max(w); h:(sum h)]
    float hull_w = 0.0f;
    float hull_h = 0.0f;

    // Now can compute layout overall size with current style
    for (auto& widget : m_widgets) {
        if (widget->tag() == Style::Tag::Layout) {
            auto layout = std::dynamic_pointer_cast<Layout>(widget);
            if (!layout)
                continue;

            layout->refreshSize();
        }

        // Get the centroid
        hull_w = std::max(widget->w(), hull_w);
        hull_h += widget->h();
    }

    return glm::vec2(hull_w, hull_h);
}
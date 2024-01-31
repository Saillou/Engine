#include "HorizontalLayout.hpp"

HorizontalLayout::HorizontalLayout(Scene& scene, Layout::ContainerType extType) :
    Layout(scene),
    m_extType(extType)
{
    // ...
}

void HorizontalLayout::draw(Scene& scene) {
    float x0 = _parent ? _parent->x() : 0.0f;
    float y0 = _parent ? _parent->y() : 0.0f;
    float w0 = _parent ? _parent->w() : 1.0f;
    float h0 = _parent ? _parent->h() : 1.0f;

    // Position
    float xi = (x0 + x()) + (w0 - w()) / 2.0f;

    for (auto& widget : m_widgets) {
        widget->x() = xi;
        widget->y() = y0 + y();
        widget->draw(m_scene);

        xi += widget->w() + padding;
    }
}

glm::vec2 HorizontalLayout::_compute_hull_size() {
    if (m_widgets.empty()) 
        return glm::vec2(0,0);

    // It's a horizontal layout, hull: [w:sum(w); h:(max h)]
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
        hull_w += widget->w() + padding;
        hull_h = std::max(widget->h(), hull_h);
    }

    return glm::vec2(hull_w, hull_h);
}
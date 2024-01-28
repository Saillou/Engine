#include "HorizontalLayout.hpp"

HorizontalLayout::HorizontalLayout(Scene& scene, Layout::ContainerType extType) :
    Layout(scene),
    m_extType(extType)
{
    // ...
}

void HorizontalLayout::draw(Scene& scene) {
    float w0 = _parent ? _parent->w() : 1.0f;
    float h0 = _parent ? _parent->h() : 1.0f;

    printf("-- horizontal layout draw -- \n");
    printf("w0:%lf h0:%lf \n", w0, h0);
    printf("w:%lf h:%lf \n", w(), h());

    // Position
    float xi = x() + (w0 - w()) / 2.0f;

    for (auto& widget : m_widgets) {
        widget->x() = x() + xi;
        widget->y() = y() + (h0 - widget->h()) / 2.0f;
        widget->draw(m_scene);

        xi += widget->w();
        printf(" - x:%lf y:%lf \n", widget->x(), widget->y());
    }
    printf("---- \n");
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
        hull_w += widget->w();
        hull_h = std::max(widget->h(), hull_h);
    }

    return glm::vec2(hull_w, hull_h);
}
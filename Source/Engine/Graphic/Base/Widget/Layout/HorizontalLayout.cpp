#include "HorizontalLayout.hpp"

HorizontalLayout::HorizontalLayout(Scene& scene) :
    Layout(scene)
{
    // ...
}

void HorizontalLayout::add(std::shared_ptr<Widget> widget, const std::string& id) {
    widget->x() = 0.0f;
    widget->y() = 0.0f;

    Layout::add(widget, id);
}


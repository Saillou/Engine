#include "VerticalLayout.hpp"

VerticalLayout::VerticalLayout(Scene& scene) :
    Layout(scene)
{
    // ...
}

void VerticalLayout::add(std::shared_ptr<Widget> widget, const std::string& id) {
    widget->x() = 0.0f;
    widget->y() = 0.0f;

    Layout::add(widget, id);
}

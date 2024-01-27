#include "Layout.hpp"

Layout::Layout(Scene& scene) :
    Widget(Style::Tag::Layout, EventListened::MouseMove | EventListened::MouseButton),
    m_scene(scene)
{
    // ..
}

// Methods
void Layout::add(std::shared_ptr<Widget> widget, const std::string& id) {
    widget->_parent = this;

    m_widgets.push_back(widget);
    m_widgets_id[id] = widget;
}
void Layout::add(std::shared_ptr<Widget> widget, float x, float y, const std::string& id) {
    widget->x() = x;
    widget->y() = y;

    add(widget, id);
}

void Layout::clean() {
    for (auto& widget : m_widgets)
        widget->_parent = nullptr;

    m_widgets = {};
    m_widgets_id = {};
}


// Access
Scene& Layout::scene() {
    return m_scene;
}

int Layout::width() const {
    return int(m_scene.width() * w());
}

int Layout::height() const {
    return int(m_scene.height() * h());
}

StyleSheet& Layout::styleSheet() {
    return m_stylesheet;
}

void Layout::draw(Scene& scene) {
    if (_parent) {
        m_stylesheet = _parent->m_stylesheet;
        m_stylesheet.applyStyle(this);
    }

    for (auto& widget : m_widgets) {
        m_stylesheet.applyStyle(widget.get());
        widget->draw(m_scene);
    }
}

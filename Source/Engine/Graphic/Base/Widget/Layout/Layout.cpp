#include "Layout.hpp"
#include <iostream>

Layout::Layout(Scene& scene) :
    Widget(Style::Tag::Layout, EventListened::MouseMove | EventListened::MouseButton),
    m_scene(scene)
{
    // ..
}

// Methods
void Layout::add(std::shared_ptr<Widget> widget, const std::string& id) {
    _setParent(widget.get(), this);

    m_widgets.push_back(widget);

    if (id.empty())
        return;

    if (m_widgets_id.find(id) != m_widgets_id.cend())
        std::cerr << "[Warning] Overriding currend widget id" << std::endl;
    
    widget->idName = id;
    m_widgets_id[id] = widget;
}
void Layout::add(std::shared_ptr<Widget> widget, float x, float y, const std::string& id) {
    widget->x() = x;
    widget->y() = y;

    Layout::add(widget, id);
}

void Layout::clean() {
    for (auto& widget : m_widgets) {
        widget->idName.clear();
        _setParent(widget.get(), nullptr);
    }

    m_widgets = {};
    m_widgets_id = {};
}


void Layout::_on_mouse_button(const CommonEvents::MouseButton& evt) {
    Widget::_on_mouse_button(evt);

    for (auto& widget : m_widgets) {
        if (widget->_evt_accepted & EventListened::MouseButton) {
            widget->_on_mouse_button(evt);
        }
    }
}

void Layout::_on_mouse_move(const CommonEvents::MouseMoved& evt) {
    Widget::_on_mouse_move(evt);

    for (auto& widget : m_widgets) {
        if (widget->_evt_accepted & EventListened::MouseMove) {
            widget->_on_mouse_move(evt);
        }
    }
}

// Access
Scene& Layout::scene() {
    return m_scene;
}

void Layout::refreshSize() {
    glm::vec2 size = _compute_hull_size();
    w() = size.x;
    h() = size.y;
}

int Layout::width() const {
    return int(m_scene.width() * w());
}

int Layout::height() const {
    return int(m_scene.height() * h());
}

int Layout::OriginalWidth() const {
    if (_parent)
        return _parent->OriginalWidth();

    return width();
}

int Layout::OriginalHeight() const {
    if (_parent)
        return _parent->OriginalHeight();

    return height();
}

StyleSheet& Layout::styleSheet() {
    return m_stylesheet;
}

void Layout::draw(Scene& scene) {
    m_scene.overlayer().quad(*_surface);

    for (auto& widget : m_widgets) {
        widget->draw(m_scene);
    }
}

void Layout::_apply_styleSheet() {
    // Propagate style
    if (_parent)
        m_stylesheet = _parent->m_stylesheet;
    
    m_stylesheet.applyStyle(this);

    for (auto& widget : m_widgets) {
        if (widget->tag() == Style::Tag::Layout) {
            auto layout = std::dynamic_pointer_cast<Layout>(widget);
            
            if (layout) {
                layout->_apply_styleSheet();
                continue;
            }
        }

        m_stylesheet.applyStyle(widget.get());
    }
}

glm::vec2 Layout::_compute_hull_size() {
    if (m_widgets.empty())
        return glm::vec2(0, 0);

    // It's a free layout, so AABB == hull
    glm::vec2 min_vert(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    glm::vec2 max_vert(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

    // Now can compute layout overall size with current style
    for (auto& widget : m_widgets) {
        if (widget->tag() == Style::Tag::Layout) {
            auto layout = std::dynamic_pointer_cast<Layout>(widget);
            if (!layout)
                continue;

            layout->refreshSize();
        }

        // Get the centroid
        min_vert = glm::min(min_vert, widget->getTL());
        max_vert = glm::max(max_vert, widget->getBR());
    }

    // Extends
    return glm::vec2(
        max_vert.x - min_vert.x, 
        max_vert.y - min_vert.y
    );
}

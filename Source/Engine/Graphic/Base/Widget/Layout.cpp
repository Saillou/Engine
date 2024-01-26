#include "Layout.hpp"


Layout::Layout(Scene& scene) :
    m_scene(scene),
    m_frame(Framebuffer::Multisample, scene.width(), scene.height()),
    m_copyFilter(scene.width(), scene.height())
{
    // Events
    _subscribe(&Layout::_on_draw);
    _subscribe([=](const SceneEvents::Resized& size) {
        m_frame.resize(size.width, size.height);
        m_copyFilter.resize(size.width, size.height);
    });
}

// Methods
void Layout::add(std::shared_ptr<Widget> widget) {
    widget->_parent = this;

    m_widgets.push_back(widget);
}

void Layout::add(std::shared_ptr<Widget> widget, float x, float y) {
    widget->_parent = this;
    widget->x() = x;
    widget->y() = y;

    m_widgets.push_back(widget);
}

void Layout::clean() {
    for (auto& widget : m_widgets)
        widget->_parent = nullptr;

    m_widgets = {};
}

// Access
Scene& Layout::scene() {
    return m_scene;
}

int Layout::width() const {
    return int(m_frame.width() * w());
}

int Layout::height() const {
    return int(m_frame.height() * h());
}

void Layout::draw(Scene& scene) {
    for (auto& widget : m_widgets) {
        widget->draw(m_scene);
    }
}

// Callbacks
void Layout::_on_draw(const SceneEvents::RenderFinished&) {
    // The parent will call the draw and we don't want it twice
    if (_parent)
        return;

    m_frame.bind();
    {
        m_frame.texture().bind();
        glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Event::Emit(LayoutEvents::Draw(), this);
        {
            draw(m_scene);
        }
        m_frame.texture().unbind();
    }
    m_frame.unbind();

    // Draw frame
    m_copyFilter.apply(m_scene.framebuffer_main(), m_frame, opacity, background_color);
    m_scene.drawFrame(m_copyFilter.result());
}
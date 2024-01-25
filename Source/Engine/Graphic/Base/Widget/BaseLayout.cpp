#include "BaseLayout.hpp"


BaseLayout::BaseLayout(Scene& scene) :
    m_scene(scene),
    m_frame(Framebuffer::Multisample, scene.width(), scene.height()),
    m_copyFilter(scene.width(), scene.height())
{
    // All the screen
    _surfaces.emplace_back(
        std::make_unique<Quad>()
    );

    // Events
    _subscribe(&BaseLayout::_on_resize);
    _subscribe(&BaseLayout::_draw);
}

// Methods
void BaseLayout::add(std::shared_ptr<BaseWidget> widget) {
    widget->_parent = this;

    m_widgets.push_back(widget);
}

void BaseLayout::clean() {
    m_widgets = {};
}

// Access
Scene& BaseLayout::scene() {
    return m_scene;
}

int BaseLayout::width() const {
    return m_frame.width();
}

int BaseLayout::height() const {
    return m_frame.height();
}

void BaseLayout::draw(Scene& scene) {
    for (auto& widget : m_widgets) {
        widget->draw(m_scene);
    }
}

// Callbacks
void BaseLayout::_on_resize(const SceneEvents::Resized& size) {
    m_frame.resize(size.width, size.height);
    m_copyFilter.resize(size.width, size.height);
}

void BaseLayout::_draw(const SceneEvents::RenderFinished&) {
    m_frame.bind();
    {
        m_frame.texture().bind();
        glClearColor(background_color.r, background_color.g, background_color.b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Event::Emit(LayoutEvents::Draw(), this);

        draw(m_scene);

        m_frame.texture().unbind();
    }
    m_frame.unbind();

    // Draw frame
    m_copyFilter.apply(m_scene.framebuffer_main(), m_frame, opacity, background_color);
    m_scene.drawFrame(m_copyFilter.result());
}
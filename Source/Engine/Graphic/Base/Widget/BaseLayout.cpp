#include "BaseLayout.hpp"


// ----
BaseLayout::BaseLayout(Scene& scene) :
    m_scene(scene),
    m_frame(Framebuffer::Multisample, scene.width(), scene.height()),
    m_copyFilter(scene.width(), scene.height())
{
    // Events
    _subscribe(&BaseLayout::_on_resize);
    _subscribe(&BaseLayout::_draw);
}

void BaseLayout::add(std::shared_ptr<BaseWidget> widget) {
    m_widgets.push_back(widget);
}

Scene& BaseLayout::scene() {
    return m_scene;
}

void BaseLayout::_on_resize(const SceneEvents::Resized& size) {
    m_frame.resize(size.width, size.height);
    m_copyFilter.resize(size.width, size.height);
}

void BaseLayout::_draw(const SceneEvents::RenderFinished&) {
    m_frame.bind();
    {
        m_frame.texture().bind();
        m_frame.clear();

        _emit(LayoutEvents::Draw());

        for (auto& widget : m_widgets) {
            widget->draw(m_scene);
        }

        m_frame.texture().unbind();
    }
    m_frame.unbind();

    // Draw frame
    m_copyFilter.apply(m_scene.framebuffer_main(), m_frame, opacity);
    m_scene.drawFrame(m_copyFilter.result());
}
#include "BaseWidget.hpp"


// ----
BaseWidget::BaseWidget(Scene& scene) :
    m_scene(scene),
    m_frame(Framebuffer::Multisample, scene.width(), scene.height()),
    m_copyFilter(scene.width(), scene.height()),
    m_callback_draw(nullptr)
{
    // Events
    _subscribe(&BaseWidget::_on_resize);
    _subscribe(&BaseWidget::_draw);
}

Scene& BaseWidget::scene() {
    return m_scene;
}

std::function<void(void)>& BaseWidget::on_draw() {
    return m_callback_draw;
}


void BaseWidget::_on_resize(const SceneEvents::Resized& size) {
    m_frame.resize(size.width, size.height);
    m_copyFilter.resize(size.width, size.height);
}

void BaseWidget::_draw(const SceneEvents::RenderFinished&) {
    // Ui frame:
    m_frame.bind();
    {
        m_frame.texture().bind();
        if (m_callback_draw) {
            m_callback_draw();
        }
        m_frame.texture().unbind();
    }
    m_frame.unbind();

    // Draw frame
    m_copyFilter.apply(m_scene.framebuffer_main(), m_frame, opacity);
    m_scene.drawFrame(m_copyFilter.result());
}
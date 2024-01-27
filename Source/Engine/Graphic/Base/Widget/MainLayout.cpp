#include "MainLayout.hpp"

MainLayout::MainLayout(Scene& scene) :
    Layout(scene),
    m_frame(Framebuffer::Multisample, scene.width(), scene.height()),
    m_copyFilter(scene.width(), scene.height())
{
    // Events
    _subscribe(&MainLayout::_on_draw);
    _subscribe([=](const SceneEvents::Resized& size) 
    {
        m_frame.resize(size.width, size.height);
        m_copyFilter.resize(size.width, size.height);
    });
}

// Callbacks
void MainLayout::_on_draw(const SceneEvents::RenderFinished&) {
    styleSheet().applyStyle(this);

    // Draw on internal framebuffer
    m_frame.bind();
    {
        m_frame.texture().bind();
        glClearColor(style().background().r, style().background().g, style().background().b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Event::Emit(LayoutEvents::Draw(), this);
        {
            draw(scene());
        }
        m_frame.texture().unbind();
    }
    m_frame.unbind();

    // Draw frame on screen
    m_copyFilter.apply(scene().framebuffer_main(), m_frame, style().opacity(), style().background());
    scene().drawFrame(m_copyFilter.result());
}

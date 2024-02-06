#include "SceneFrame.hpp"

SceneFrame::SceneFrame(Scene& scene_, std::shared_ptr<Layout> layout_) :
    m_scene(scene_),
    m_frame(Framebuffer::Multisample, scene_.width(), scene_.height()),
    m_copyFilter(scene_.width(), scene_.height())
{
    m_layout = layout_ ? layout_ : std::make_unique<Layout>(scene_);

    // - Events -
    _subscribe(&SceneFrame::_on_draw);

    // Notes: Order has its importance
    //   Mouse move will detect if the mouse is over the widget
    //  then mouse button will trigger or not if inside of it.

    _subscribe([=](const CommonEvents::MouseMoved& evt) { 
        m_layout->_on_mouse_move(evt);   
    });
    _subscribe([=](const CommonEvents::MouseButton& evt) { 
        m_layout->_on_mouse_button(evt); 
    });
    _subscribe([=](const SceneEvents::Resized& size) {
        m_frame.resize(size.width, size.height);
        m_copyFilter.resize(size.width, size.height);
    });
}

Layout& SceneFrame::layout() {
    return *m_layout.get();
}

Scene& SceneFrame::scene() {
    return m_scene;
}

// Callbacks
void SceneFrame::_on_draw(const SceneEvents::RenderFinished&) {
    // Refresh style data
    m_layout->_apply_styleSheet();
    m_layout->_compute_hull_size();

    // Draw on internal framebuffer
    m_frame.bind();
    {
        m_frame.texture().bind();
        glClearColor(m_layout->style().background().r, m_layout->style().background().g, m_layout->style().background().b, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Event::Emit(WidgetEvents::Draw(), this);
        {
            m_layout->draw(m_scene);
        }
        m_frame.texture().unbind();
    }
    m_frame.unbind();

    // Draw frame on screen
    m_copyFilter.apply(m_scene.framebuffer_main(), m_frame, m_layout->style().opacity(), m_layout->style().background());
    m_scene.drawFrame(m_copyFilter.result());
}

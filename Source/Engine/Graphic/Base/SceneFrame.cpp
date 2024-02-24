#include "SceneFrame.hpp"

SceneFrame::SceneFrame(Scene& scene_, std::shared_ptr<Layout> layout_) :
    m_scene(scene_)
{
    m_layout = layout_ ? layout_ : std::make_unique<Layout>(scene_);

    // - Events -
    _subscribe([=](const CommonEvents::MouseButton& evt) { 
        m_layout->_on_mouse_button(evt); 
    });
    _subscribe([=](const CommonEvents::MouseMoved& evt) { 
        m_layout->_on_mouse_move(evt);   
    });

    // TODO: do it with component instead
    m_scene.overlayer().add_frame(this);
}

SceneFrame::~SceneFrame()
{
    // TODO: same
    m_scene.overlayer().remove_frame(this);
}

Layout& SceneFrame::layout() {
    return *m_layout.get();
}

// Callbacks
void SceneFrame::draw() {
    m_layout->_apply_styleSheet();
    m_layout->_applyStyle();
    m_layout->_compute_hull_size();
    m_layout->draw(m_scene);

    Event::Emit(WidgetEvents::Draw(), this);
}

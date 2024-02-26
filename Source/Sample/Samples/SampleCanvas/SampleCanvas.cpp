#include "SampleCanvas.hpp"

SampleCanvas::SampleCanvas() :
    m_scene(Service<Window>::get().scene())
{
    m_scene.camera.direction = glm::vec3(10, 2, 0);
    m_scene.camera.position  = glm::vec3(10, -5, 3);

    // Enable events
    _subscribe(&SampleCanvas::_on_key_pressed);
    _subscribe(&SampleCanvas::_update);

    m_timer.tic();
}

// - Events -
void SampleCanvas::_update(const CommonEvents::StateUpdated&)
{

}

void SampleCanvas::_on_key_pressed(const CommonEvents::KeyPressed& evt) 
{
    if (evt.action != InputAction::Pressed && evt.action != InputAction::Repeated)
        return;

    const float MIN_CAM_X = 5.0f;
    const float MAX_CAM_X = 15.0f;

    float move_x  = 0.0f;
    float speed_x = 0.1f;

    switch (evt.key)
    {
        // Camera control
        case KeyCode::ArrowLeft:  move_x = -1.0f; break;
        case KeyCode::ArrowRight: move_x = +1.0f; break;

        // Game actions
        case KeyCode::Escape: wantQuit = true;
    }

    float next_x = m_scene.camera.direction.x + speed_x * move_x;
    if (next_x < MAX_CAM_X && next_x > MIN_CAM_X)
    {
        m_scene.camera.direction.x = next_x;
        m_scene.camera.position.x  = next_x;
    }

}

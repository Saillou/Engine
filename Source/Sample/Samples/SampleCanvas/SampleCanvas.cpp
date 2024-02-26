#include "SampleCanvas.hpp"

SampleCanvas::SampleCanvas() :
    m_scene(Service<Window>::get().scene())
{
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

}

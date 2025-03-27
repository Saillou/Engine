#include "Game.hpp"

#include <Engine/Framework/Service.hpp>
#include <Engine/Graphic/Window.hpp>
#include <Engine/Graphic/Base/Widget/Text.hpp>

struct DataTest {
    int count;
} G_data_test;

Game::Game() :
    m_scene(Service<Window>::get().scene()),
    m_canvas(CanvasElement()),
    m_frame(SceneFrame(m_scene))
{
    // setup
    m_frame.layout().style().background.setValue(Style::Transparent());

    m_canvas.canvas()
        .ctx()
        .dimensions(CanvasContext::Dimensions::Relative)
        .colors(CanvasContext::ColorFormat::Uint8);

    // Draw
    m_frame.layout().add(Text::Create(std::to_string(G_data_test.count)), 0.7f, 0.3f, "#counter");

    m_canvas.canvas().clear();
    m_canvas.canvas()
        .begin()
        .rect(-0.5f, 0.25f, 0.25f, 0.25f)
        .fill(glm::vec4(42, 42, 42, 255))
        .stroke(glm::vec4(255, 255, 255, 127), 2.0f);

    _subscribe(&Game::_on_key_pressed);
    _subscribe(&Game::_on_click);
    _subscribe(&Game::_state_updated);
}
Game::~Game() {
    _unsubscribeAll();
}

bool Game::wantQuit() const
{
	return _want_quit;
}

void Game::_state_updated(const CommonEvents::StateUpdated&)
{


}

void Game::_on_key_pressed(const CommonEvents::KeyPressed& evt) 
{
    switch (evt.key)
    {
        case KeyCode::Escape: _want_quit = true; break;
    }
}

void Game::_on_click(const CommonEvents::MouseButton& btn)
{
    if (btn.button == MouseButton::Left) {
        if (btn.action == InputAction::Pressed) {
            auto text_count = m_frame.layout().find<Text>("#counter");
            if (text_count) {
                text_count->setText(std::to_string(++G_data_test.count));
            }

            m_canvas.canvas().clear();
            m_canvas.canvas()
                .begin()
                .rect(-0.5f, 0.25f, 0.25f, 0.25f)
                .fill(glm::vec4(42, 42, 42, 255))
                .stroke(glm::vec4(255, 255, 255, 127), 2.0f);

            m_canvas.canvas()
                .begin()
                .circle(-0.5f + (rand() % 1000) / 1000.0f, 0.25f + (rand()%1000)/1000.0f, 0.25f + (rand() % 1000) / 1000.0f)
                .fill(glm::vec4(42, 42, 42, 255))
                .stroke(glm::vec4(255, 255, 255, 127), 2.0f);

        }
    }
}

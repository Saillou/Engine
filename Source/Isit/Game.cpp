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
    // Setup
    m_frame.layout().style().background.setValue(Style::Transparent());

    m_canvas.canvas()
        .ctx()
        .dimensions(CanvasContext::Dimensions::Absolute)
        .colors(CanvasContext::ColorFormat::Uint8);

    m_button_counter = CircleButton { 300, 300, 50 };
    m_square_animation.tweet_load = Animator::Tweet( 0.0f, m_square_animation.load_time, Animator::Tweet::Type::Quadratic );
    m_square_animation.tweet_fade = Animator::Tweet( 0.0f, 0.3f, Animator::Tweet::Type::Quadratic );

    // Draw
    m_frame.layout().add(Text::Create(std::to_string(G_data_test.count)), 300.0f/ m_scene.width(), 300.0f / m_scene.height(), "#counter");

    _redraw_canvas();

    // Events
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
    _redraw_canvas();
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
            m_button_counter.is_pressed =
                (btn.x - m_button_counter.x) * (btn.x - m_button_counter.x) +
                (btn.y - m_button_counter.y) * (btn.y - m_button_counter.y)
                <= m_button_counter.r * m_button_counter.r;
        }
        
        if (btn.action == InputAction::Released) {
            if (m_button_counter.is_pressed) {
                if (m_square_animation.tweet_playing != 1) {
                    G_data_test.count++;

                    auto text_count = m_frame.layout().find<Text>("#counter");
                    if (text_count) {
                        text_count->setText(std::to_string(G_data_test.count));
                    }

                    if (m_square_animation.load_time > 0.1f) {
                        m_square_animation.load_time -= 0.1f;
                    }

                    m_square_animation.tweet_load = Animator::Tweet(0.0f, m_square_animation.load_time, Animator::Tweet::Type::Quadratic);
                    m_square_animation.tweet_playing = 1;
                }
            }

            m_button_counter.is_pressed = false;
        }
    }
}

void Game::_redraw_canvas()
{
    const int WIDTH = m_scene.width();
    const int HEIGHT = m_scene.height();

    m_canvas.canvas().clear();

    m_canvas.canvas()
        .begin()
        .circle(m_button_counter.x, (float)HEIGHT - m_button_counter.y, m_button_counter.r)
        .fill(m_button_counter.is_pressed ? glm::vec4(42, 142, 42, 255) : glm::vec4(42, 42, 42, 255))
        .stroke(glm::vec4(255, 255, 255, 127), 2.0f);

    m_canvas.canvas()
        .begin()
        .rect(75, (float)HEIGHT - 350, 150, 100)
        .fill(glm::vec4(42, 42, 42, 255))
        .stroke(glm::vec4(255, 255, 255, 127), 2.0f);

    // Animations
    switch (m_square_animation.tweet_playing) 
    {
    case 1: {
        int green = m_square_animation.tweet_load.update(42, 255);
        int width = m_square_animation.tweet_load.update(0, 150);

        m_canvas.canvas().begin()
            .rect(75, HEIGHT - 350, width, 100)
            .fill(glm::vec4(42, green, 42, 255))
            .stroke(glm::vec4(255, 255, 255, 127), 2.0f);

        if (m_square_animation.tweet_load.ended()) {
            m_square_animation.tweet_playing = 2;
            m_square_animation.tweet_fade.reset();
        }
    } break;
    case 2: {
        int green = m_square_animation.tweet_fade.update(255, 42);

        m_canvas.canvas().begin()
            .rect(75, HEIGHT - 350, 150, 100)
            .fill(glm::vec4(42, green, 42, 255))
            .stroke(glm::vec4(255, 255, 255, 127), 2.0f);

        if (m_square_animation.tweet_fade.ended()) {
            m_square_animation.tweet_playing = 0;
        }
    } break;
    }
}

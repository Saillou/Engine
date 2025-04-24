#pragma once

#include "PanelIdle.hpp"

PanelIdle::PanelIdle(const Scene& scene) :
    Panel(),
    m_button_story(Rectangle{ 
        (float)scene.width() - 200.0f, 30.0f, 100, 70 },
        "Story", 
        CanvasShape::Color(142, 142, 142, 255)
    )
{
    m_button_counter = { 300, 300, 50, 50 };
    m_square_animation.tweet_load = Animator::Tweet(0.0f, m_square_animation.load_time, Animator::Tweet::Type::Quadratic);
    m_square_animation.tweet_fade = Animator::Tweet(0.0f, 0.3f, Animator::Tweet::Type::Quadratic);

    _subscribe(&m_button_story, [=](const CommonEvents::MouseButton& btn) {
        if (btn.action != InputAction::Pressed)
            return;

        Event::Emit(IdleEvents::Back(), this);
    });
}

void PanelIdle::draw(CanvasEntity& canvas, const Scene& scene)
{
    const int WIDTH  = scene.width();
    const int HEIGHT = scene.height();

    canvas.get()
        .begin()
        .circle(m_button_counter.x, (float)HEIGHT - m_button_counter.y, m_button_counter.w)
        .fill(m_button_counter.is_pressed ? CanvasShape::Color(42, 142, 42, 255) : CanvasShape::Color(42, 42, 42, 255))
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);

    canvas.get()
        .begin()
        .rect(75, (float)HEIGHT - 350, 150, 100)
        .fill(CanvasShape::Color(42, 42, 42, 255))
        .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);

    canvas.get()
        .begin()
        .text(std::to_string(shared_data().idle.count), m_button_counter.x, (float)HEIGHT - m_button_counter.y, 0.5f)
        .fill(CanvasShape::Color(142, 142, 142, 255));

    m_button_story.draw();

    // Animations
    switch (m_square_animation.tweet_playing)
    {
    case 1:
    {
        float green = m_square_animation.tweet_load.update(42.0f, 255.0f);
        float width = m_square_animation.tweet_load.update(0.0f, 150.0f);

        canvas.get().begin()
            .rect(75.0f, HEIGHT - 350.0f, width, 100.0f)
            .fill(CanvasShape::Color(42, green, 42, 255))
            .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);

        if (m_square_animation.tweet_load.ended()) {
            m_square_animation.tweet_playing = 2;
            m_square_animation.tweet_fade.reset();
        }
    } break;
    case 2:
    {
        int green = m_square_animation.tweet_fade.update(255, 42);

        canvas.get().begin()
            .rect(75.0f, HEIGHT - 350.0f, 150.0f, 100.0f)
            .fill(CanvasShape::Color(42, green, 42, 255))
            .stroke(CanvasShape::Color(255, 255, 255, 127), 2.0f);

        if (m_square_animation.tweet_fade.ended()) {
            m_square_animation.tweet_playing = 0;
        }
    } break;
    }
}
void PanelIdle::reset() {
    m_button_story.clear();
}
void PanelIdle::mouse_clicked(const CommonEvents::MouseButton& btn) {
    if (btn.button == MouseButton::Left) {
        if (btn.action == InputAction::Pressed) {
            m_button_counter.is_pressed =
                (btn.x - m_button_counter.x) * (btn.x - m_button_counter.x) +
                (btn.y - m_button_counter.y) * (btn.y - m_button_counter.y)
                <= m_button_counter.w * m_button_counter.h;
        }

        if (btn.action == InputAction::Released) {
            if (m_button_counter.is_pressed) {
                if (m_square_animation.tweet_playing != 1) {
                    shared_data().idle.count++;

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
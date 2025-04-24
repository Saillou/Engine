#pragma once

#include "PanelStory.hpp"

PanelStory::PanelStory(const Scene& scene) :
    Panel(),
    m_button_idle(Rectangle{ 
        (float)scene.width() / 2.0f - 100.0f, 400, 100, 70 }, 
        "Idle", 
        CanvasShape::Color(142, 142, 142, 32)
    ),
    m_button_fight(Rectangle{ 
        (float)scene.width() / 2.0f + 100.0f, 400, 100, 70 }, 
        "Fight", 
        CanvasShape::Color(142, 142, 142, 255)
    )
{
    m_text_animation.tweet_fade_in_1 = Animator::Tweet(0.0f, 1.0f, Animator::Tweet::Type::Quadratic);
    m_text_animation.tweet_fade_in_2 = Animator::Tweet(0.7f, 1.0f, Animator::Tweet::Type::Quadratic);

    _subscribe(&m_button_idle, [=](const CommonEvents::MouseButton& btn) {
        if (btn.action != InputAction::Pressed)
            return;

        Event::Emit(StoryEvents::Choice(StoryEvents::Choice::Idle), this);
    });

    _subscribe(&m_button_fight, [=](const CommonEvents::MouseButton& btn) {
        if (btn.action != InputAction::Pressed)
            return;

        Event::Emit(StoryEvents::Choice(StoryEvents::Choice::Fight), this);
    });

    reset();
}

void PanelStory::reset()
{
    m_text_animation.tweet_fade_in_1.reset();
    m_text_animation.tweet_fade_in_2.reset();

    m_button_idle.clear();
    m_button_fight.clear();
}

void PanelStory::draw(CanvasEntity& canvas, const Scene& scene)
{
    const float WIDTH = (float)scene.width();
    const float HEIGHT = (float)scene.height();

    float alpha_1 = m_text_animation.tweet_fade_in_1.update(0.f, 1.0f);
    float alpha_2 = m_text_animation.tweet_fade_in_2.update(0.f, 1.0f);

    canvas.get()
        .begin()
        .text("Lorem ipsum dolor sit amet,", WIDTH / 3.0f + 50.0f, HEIGHT / 1.5f, 0.5f)
        .fill(CanvasShape::Color(142, 142, 142, int(255 * alpha_1)))
        .text("consectetur adipiscing elit. ", WIDTH / 3.0f + 50.0f, HEIGHT / 1.5f - 40, 0.5f)
        .fill(CanvasShape::Color(142, 142, 142, int(255 * alpha_2)));

    if (m_text_animation.tweet_fade_in_2.ended())
    {
        m_button_idle.draw();
        m_button_fight.draw();
    }
}

void PanelStory::mouse_clicked(const CommonEvents::MouseButton& btn) 
{
    
}
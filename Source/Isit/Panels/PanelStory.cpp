#pragma once

#include "PanelStory.hpp"

PanelStory::PanelStory()
{
    // Sprites
    m_text_animation.tweet_fade_in_1 = Animator::Tweet(0.0f, 1.0f, Animator::Tweet::Type::Quadratic);
    m_text_animation.tweet_fade_in_2 = Animator::Tweet(0.7f, 1.0f, Animator::Tweet::Type::Quadratic);

    add("button_idle", std::make_shared<Button>(Rectangle{
        width() / 2.0f - 100.0f, 400, 100, 70 },
        "Idle",
        CanvasShape::Color(142, 142, 142, 32)
    ));

    add("button_fight", std::make_shared<Button>(Rectangle{
        width() / 2.0f + 100.0f, 400, 100, 70 },
        "Fight",
        CanvasShape::Color(142, 142, 142, 255)
    ));

    // Events
    _subscribe(get("button_idle"), [=](const CommonEvents::MouseButton& btn) {
        if (btn.action != InputAction::Pressed)
            return;

        Event::Emit(PanelStory::Events::Choice(PanelStory::Events::Choice::Idle), this);
    });

    _subscribe(get("button_fight"), [=](const CommonEvents::MouseButton& btn) {
        if (btn.action != InputAction::Pressed)
            return;

        Event::Emit(PanelStory::Events::Choice(PanelStory::Events::Choice::Fight), this);
    });

    reset();
}

void PanelStory::reset()
{
    m_text_animation.tweet_fade_in_1.reset();
    m_text_animation.tweet_fade_in_2.reset();
}

void PanelStory::draw()
{
    float alpha_1 = m_text_animation.tweet_fade_in_1.update(0.f, 1.0f);
    float alpha_2 = m_text_animation.tweet_fade_in_2.update(0.f, 1.0f);

    _canvas.get()
        .begin()
        .text("A mortal path to eternity,", width() / 3.0f + 50.0f, height() / 1.5f, 0.5f)
        .fill(CanvasShape::Color(142, 142, 142, int(255 * alpha_1)))
        .text("can only meet hardships. ", width() / 3.0f + 50.0f, height() / 1.5f - 40, 0.5f)
        .fill(CanvasShape::Color(142, 142, 142, int(255 * alpha_2)));

    if (m_text_animation.tweet_fade_in_2.ended())
    {
        get("button_idle")->draw();
        get("button_fight")->draw();
    }
}
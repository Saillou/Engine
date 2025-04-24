#pragma once

#include "Panel.hpp"

struct IdleEvents : public Event
{
    struct Back : public _Base
    {
        Back() : _Base(_Type::EventBack){ }
    };

private:
    enum _Type : int {
        EventBack,
    };
};

struct PanelIdle : public Panel
{
    explicit PanelIdle(const Scene& scene);
    void draw(CanvasEntity& canvas, const Scene& scene) override;
    void reset() override;
    void mouse_clicked(const CommonEvents::MouseButton& btn) override;

private:
    Button m_button_story;
    Rectangle m_button_counter;

    struct {
        Animator::Tweet tweet_load;
        Animator::Tweet tweet_fade;
        int tweet_playing = 0;
        float load_time = 2.0f;
    } m_square_animation;
};
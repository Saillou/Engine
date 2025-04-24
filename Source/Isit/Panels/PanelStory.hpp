#pragma once

#include "Panel.hpp"

struct StoryEvents : public Event
{
    struct Choice : public _Base
    {
        enum _list_choices {
            Unkwnown,
            Idle,
            Fight,
        } _choice;

        Choice(_list_choices r = Unkwnown) :
            _Base(_Type::EventChoice), _choice(r)
        {
        }
    };

private:
    enum _Type : int {
        EventChoice,
    };
};

struct PanelStory : public Panel
{
    explicit PanelStory(const Scene& scene);
    void reset() override;
    void draw(CanvasEntity& canvas, const Scene& scene) override;
    void mouse_clicked(const CommonEvents::MouseButton& btn) override;

private:
    Button m_button_idle;
    Button m_button_fight;

    struct {
        Animator::Tweet tweet_fade_in_1;
        Animator::Tweet tweet_fade_in_2;
    } m_text_animation;
};
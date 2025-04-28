#pragma once

#include "Panel.hpp"

struct PanelStory : public Panel
{
    struct Events : public Event
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

    PanelStory();
    void reset() override;
    void draw() override;

private:
    struct {
        Animator::Tweet tweet_fade_in_1;
        Animator::Tweet tweet_fade_in_2;
    } m_text_animation;
};
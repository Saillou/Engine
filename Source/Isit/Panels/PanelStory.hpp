#pragma once

#include "Panel.hpp"
#include "Story/StoryManager.hpp"

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
    //void draw() override;

protected:
    void _state_updated(const CommonEvents::StateUpdated&);
    //void onMousePressed() override;
    //void onMouseReleased() override;
    //void onMouseMoved(int x, int y) override;

private:
    void _apply_model();

    //void _draw_node(StoryEngine::Node&);
    //void _draw_element(float x0, float y0, const std::string& text, float size_text);
    //Rectangle _create_bounding_box(float x0, float y0, const std::string& text, float size_text) const;
    //bool _check_input(StoryEngine::Node&, const StoryEngine::Element&, const Rectangle&);
    //bool _draw_elements(StoryEngine::Node&);

    StoryManager& m_story;
    AnimationManager& animation_manager;
    //Animator::Tweet m_tweet_story;

    bool m_need_refresh_page = true;
    //bool m_current_mouse_state = false;
    //int m_current_mouse_x      = 0;
    //int m_current_mouse_y      = 0;
};
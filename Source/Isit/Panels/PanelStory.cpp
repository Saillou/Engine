#pragma once

#include "PanelStory.hpp"

static const std::unordered_map<std::string, PanelStory::Events::Choice> PanelName = {
    {"idle", PanelStory::Events::Choice::Idle },
    {"fight", PanelStory::Events::Choice::Fight }
};
static const CanvasShape::Color TxtColor = CanvasShape::Color(255, 255, 255, 255);

PanelStory::PanelStory():
    m_story(StoryManager::Instance()),
    animation_manager(AnimationManager::Instance())
    //m_tweet_story(Animator::Tweet(0.0f, 0.5f, Animator::Tweet::Type::Linear))
{
    reset();

    _subscribe(&PanelStory::_state_updated);
}

void PanelStory::_state_updated(const CommonEvents::StateUpdated&) {
    if (m_need_refresh_page) {
        m_need_refresh_page = false;
        _apply_model();
    }

    if (animation_manager.Current() && animation_manager.Current()->ended()) {
        animation_manager.Reset();
        m_story.current_node().next();
        m_need_refresh_page = true;
    }
}

void PanelStory::reset()
{
    clear();
    //m_fresh_page            = true;
    //m_current_mouse_state   = false;
    //m_tweet_story.reset();
    animation_manager.Reset();
}

//void PanelStory::draw()
//{
//    if (m_tweet_story.ended()) {
//        //std::dynamic_pointer_cast<Text>(get("text_1"))->text() = "Text_2";
//        remove("text_1");
//    }
//
//    if (get("text_1")) {
//        get("text_1")->draw();
//    }
//    //_draw_node(m_story.current_node());
//}

//void PanelStory::onMousePressed()
//{
//    m_current_mouse_state = true;
//}
//void PanelStory::onMouseReleased()
//{
//    m_current_mouse_state = false;
//}
//void PanelStory::onMouseMoved(int x, int y) {
//    m_current_mouse_x = x;
//    m_current_mouse_y = y;
//}

//Rectangle PanelStory::_create_bounding_box(float x0, float y0, const std::string& text, float size_text) const {
//    auto text_measure = TextEngine::Measure(text, size_text);
//    float x = x0 - 5.0f;
//    float y = y0 - text_measure.y + 5.0f;
//    float w = text_measure.x + 10.0f;
//    float h = text_measure.y + 10.0f;
//
//    return { x,y,w,h };
//}

void PanelStory::_apply_model() 
{
    remove_all();

    const float x0 = 50.0f;
    const float y0 = 50.0f;
    const float pad_h = 50.0f;

    size_t i_element = 0;
    do {
        for (auto& elt : m_story.current_node().current_io().elements)
        {
            if (elt.condition && !elt.condition()) {
                continue;
            }
            if (elt.action) {
                elt.action();
            }
            if (animation_manager.Current()) {
                animation_manager.Current()->play(add("sprite_animation"));
                break;
            }

            const std::string sprite_name   = "text_" + std::to_string(i_element);
            const Point sprite_pos          = Point{ 50.0f, y0 + i_element * pad_h };
            const auto type_tag             = m_story.current_node().current_io().type;
            auto sprite_text                = std::make_shared<Text>(sprite_pos, 0.5f, elt.content, TxtColor);
            i_element++;

            if (type_tag == StoryEngine::IO::tag::Output) {
                add(sprite_name, sprite_text);
            }
            if(type_tag == StoryEngine::IO::tag::Input) {
                auto button = add(sprite_name, std::make_shared<Button>(*sprite_text));

                // Events
                _subscribe(button, [=](const CommonEvents::MouseButton& button) {
                    if (button.action != InputAction::Pressed)
                        return;

                    switch (elt.target.type)
                    {
                    case StoryEngine::Element::Target::type::Page:
                        if (!m_story.contains(elt.target.id)) {
                            std::cerr << "Page not found: " << elt.target.id << std::endl;
                            break;
                        }
                    
                        m_story.set_page(elt.target.id);
                        m_need_refresh_page = true;
                        break;
                    
                    case StoryEngine::Element::Target::type::Panel:
                        if (PanelName.find(elt.target.id) == PanelName.cend()) {
                            std::cerr << "Panel not found: " << elt.target.id << std::endl;
                            break;
                        }
                    
                        Event::Emit(PanelStory::Events::Choice(PanelName.at(elt.target.id)), this);
                        break;
                    
                    default:
                        m_need_refresh_page = true;
                        m_story.current_node().next();
                        break;
                    }
                });
            }
        }
    } while (
        /* Continue reading until we have read an input    */    m_story.current_node().current_io().type != StoryEngine::IO::tag::Input
        /* Continue reading until we get have no node next */ && m_story.current_node().next()
        /* Wait for no animation playing                   */ && !animation_manager.Current()
    );
}

//void PanelStory::_draw_node(StoryEngine::Node& node) {
//    // Draw
//    bool need_reset = _draw_elements(node);
//    m_fresh_page = false;
//
//    if (need_reset)
//        goto label_reset;
//
//    // Check output end
//    if (node.current_io().type == StoryEngine::IO::tag::Output)
//    {
//        // Animation or tweet based ?
//        if (animation_manager.Current() ? animation_manager.Current()->ended() : m_tweet_story.ended()) {
//            need_reset = node.next();
//        }
//    }
//
//    // Reset internals / animations
//    if (need_reset)
//        label_reset:
//    reset();
//}
//bool PanelStory::_draw_elements(StoryEngine::Node& node) {
//    bool need_reset = false;
//    float offset = 50.0f;
//    for (auto& elt : node.current_io().elements)
//    {
//        if (elt.condition && !elt.condition()) {
//            continue;
//        }
//
//        if (m_fresh_page && elt.action) {
//            elt.action();
//
//            if (animation_manager.Current()) {
//                animation_manager.Current()->play(get("sprite_animation"));
//            }
//        }
//
//        if (animation_manager.Current()) {
//            animation_manager.Current()->update();
//        }
//
//        if (elt.content.empty()) {
//            continue;
//        }
//
//        constexpr float text_size = 0.5f;
//        const float x0 = 50.0f;
//        const float y0 = height() - offset;
//
//        if (node.current_io().type == StoryEngine::IO::tag::Input)
//            need_reset |= _check_input(node, elt, _create_bounding_box(x0, y0, elt.content, text_size));
//
//        _draw_element(x0, y0, elt.content, text_size);
//
//        offset += 50.0f;
//    }
//
//    return need_reset;
//}
//void PanelStory::_draw_element(float x0, float y0, const std::string& text, float size_text)
//{
//    _canvas.get()
//        .begin()
//        .text(text, x0, y0, size_text)
//        .fill(CanvasShape::Color(142, 142, 142, 220));
//}
//
//bool PanelStory::_check_input(StoryEngine::Node& node, const StoryEngine::Element& elt, const Rectangle& bounding) {
//    // Over
//    if (!bounding.is_in(m_current_mouse_x, (int)height() - m_current_mouse_y))
//        return false;
//
//    _canvas.get()
//        .begin()
//        .rect(bounding.x, bounding.y, bounding.w, bounding.h)
//        .stroke(CanvasShape::Color(142, 142, 142, 220));
//
//    // Clicked
//    if (!m_current_mouse_state)
//        return false;
//
//    switch (elt.target.type)
//    {
//    case StoryEngine::Element::Target::type::Page:
//        if (!m_story.contains(elt.target.id)) {
//            std::cerr << "Page not found: " << elt.target.id << std::endl;
//            break;
//        }
//
//        m_story.set_page(elt.target.id);
//        break;
//
//    case StoryEngine::Element::Target::type::Panel:
//        if (PanelName.find(elt.target.id) == PanelName.cend()) {
//            std::cerr << "Panel not found: " << elt.target.id << std::endl;
//            break;
//        }
//
//        Event::Emit(PanelStory::Events::Choice(PanelName.at(elt.target.id)), this);
//        break;
//
//    default:
//        node.next();
//        break;
//    }
//
//    return true;
//}

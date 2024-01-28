#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "Events/WidgetEvents.hpp"
#include "Style/Style.hpp"
#include "../Scene.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../../../Events/CommonEvents.hpp"
#include "../../../Utils/Timer.hpp"

struct Layout;

// -- Widget --
struct Widget : 
    protected Event::Subscriber
{
    friend struct StyleSheet;

    // Enums
    enum EventListened {
        None            = 0,
        MouseMove       = (1 << 0),
        MouseButton     = (1 << 1),
        KeyboardButton  = (1 << 2)
    };

    // Constructors
    template<class WidgetTag, typename ...Args>
    static std::shared_ptr<WidgetTag> Create(Args&... args)
    {
        return std::make_shared<WidgetTag>(args...);
    }

    template<const char* classname>
    struct WithClass
    {
        template<class WidgetTag, typename ...Args>
        static std::shared_ptr<WidgetTag> Create(Args&... args) {
            auto widget = std::make_shared<WidgetTag>(args...);
            widget->className = classname;

            return widget;
        }
    };

    // Methods
    virtual ~Widget() = default;
    virtual void draw(Scene&) = 0;

    // Helper
    static bool IsIn(const Widget&, int x, int y);

    // Access
    bool isMouseOver() const;
    const Layout* parent() const;

    Style::Tag tag() const;
    Style& style();
    const Style& style() const;

    // Members
    Quad& surface();
    float& x();
    float& y();
    float& w();
    float& h();

    float x() const;
    float y() const;
    float w() const;
    float h() const;

   glm::vec2 getTL() const;
   glm::vec2 getBR() const;

    std::string className = "";

protected:
    Widget(Style::Tag = Style::Tag::None, int eventListened = EventListened::None);

    // Used to know draw reference
    Layout* _parent = nullptr;
    static void _setParent(Widget* child, Layout* parent);

    // Used for draw and mouse hitbox
    std::unique_ptr<Quad> _surface;

    // Used for styling only
    void _applyStyle();

    Style      _style;
    Style::Tag _tag;

private:
    void _on_mouse_button(const CommonEvents::MouseButton& evt);
    void _on_mouse_move(const CommonEvents::MouseMoved& evt);

    bool _mouse_over = false;
};

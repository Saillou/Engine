#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "WidgetEvents.hpp"
#include "../Scene.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../../../Events/CommonEvents.hpp"
#include "../../../Utils/Timer.hpp"

// -- Widget --
struct Widget : 
    protected Event::Subscriber
{
    friend struct Layout;

    // Enums
    enum EventListened {
        None            = 0,
        MouseMove       = (1 << 0),
        MouseButton     = (1 << 1),
        KeyboardButton  = (1 << 2)
    };

    enum class Tag {
        None = 0, 
        Layout, Button, Text, 
        Custom = 400
    };

    // Methods
    template <class WidgetTag, typename ...Args>
        static std::shared_ptr<WidgetTag> Create(Args... args);

    virtual ~Widget() = default;
    virtual void draw(Scene&) = 0;

    // Helper
    static bool IsIn(const Widget&, int x, int y);
    static glm::vec4 Transparent();

    // Access
    bool isMouseOver() const;
    const Layout* parent() const;

    // Members
    float opacity = 1.0f;

    Quad& surface();
    float& x();
    float& y();
    float& w();
    float& h();

    float x() const;
    float y() const;
    float w() const;
    float h() const;

protected:
    Widget(Tag = Tag::None, int eventListened = EventListened::None);

    // Used to know draw reference
    Layout* _parent = nullptr;

    // Used for draw and mouse hitbox
    std::unique_ptr<Quad> _surface;

    // Used for styling only
    const Tag   _tag;
    std::string _idName;

private:
    void _on_mouse_button(const CommonEvents::MouseButton& evt);
    void _on_mouse_move(const CommonEvents::MouseMoved& evt);

    bool _mouse_over = false;
};

// -- Template --
template<class WidgetTag, typename ...Args>
inline std::shared_ptr<WidgetTag> Widget::Create(Args ...args)
{
    static_assert(std::is_base_of<Widget, WidgetTag>(), "Can't create non inherited BaseWidget.");

    return std::make_shared<WidgetTag>(args...);
}

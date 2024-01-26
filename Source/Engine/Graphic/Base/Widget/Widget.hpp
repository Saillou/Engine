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
    friend struct BaseLayout;

    // Enums
    enum EventListened {
        None            = 0,
        MouseMove       = (1 << 0),
        MouseButton     = (1 << 1),
        KeyboardButton  = (1 << 2)
    };

    enum class Tag {
        None = 0, 
        Button, Text, 
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
    const BaseLayout* parent() const;

    // Members
    float opacity = 1.0f;
    float x = 0.0f;
    float y = 0.0f;

protected:
    Widget(Tag = Tag::None, int eventListened = EventListened::None);

    // Used to know draw reference
    BaseLayout* _parent = nullptr;

    // Used for draw and mouse hitbox
    std::vector<std::unique_ptr<Quad>> _surfaces;

    // Used for styling only
    const Tag   _tag;
    std::string _className;
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

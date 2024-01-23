#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "../Scene.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../../../Events/CommonEvents.hpp"
#include "../../../Utils/Filter/CopyFilter.hpp"
#include "../../../Utils/Timer.hpp"

// -- Widget --
struct BaseWidget {
    virtual void draw(Scene&) = 0;

    float opacity = 1.0f;
};

// -- Layout --
struct BaseLayout : Event::Subscriber
{
    BaseLayout(Scene& scene);
    virtual ~BaseLayout() = default;

    // Methods
    void add(std::shared_ptr<BaseWidget>);

    // Setters
    Scene& scene();

    // Callbacks
    std::function<void(void)>& on_draw();

    // Members
    float opacity = 1.0f;

protected:
    virtual void _on_resize(const SceneEvents::Resized&);
    virtual void _draw(const SceneEvents::RenderFinished&);

private:
    Scene& m_scene;
    Framebuffer m_frame;
    CopyFilter m_copyFilter;
    std::function<void(void)> m_callback_draw;

    std::vector<std::shared_ptr<BaseWidget>> m_widgets;
};

#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "WidgetEvents.hpp"
#include "../Scene.hpp"
#include "../../Wrapper/Framebuffer.hpp"
#include "../../../Events/CommonEvents.hpp"
#include "../../../Utils/Filter/CopyFilter.hpp"
#include "../../../Utils/Timer.hpp"

// -- Widget --
struct BaseWidget {
    virtual void draw(Scene&) = 0;

    float opacity = 1.0f;

    static glm::vec4 Transparent() {
        return glm::vec4(0, 0, 0, 0);
    }
};

// -- Layout --
struct BaseLayout : 
    Event::Subscriber,
    Event::Emitter
{
    BaseLayout(Scene& scene);
    virtual ~BaseLayout() = default;

    // Methods
    void add(std::shared_ptr<BaseWidget>);
    void clean();

    // Setters
    Scene& scene();

    // Members
    float opacity = 1.0f;
    glm::vec4 background_color = glm::vec4(0,0,0,1);

protected:
    virtual void _on_resize(const SceneEvents::Resized&);
    virtual void _draw(const SceneEvents::RenderFinished&);

private:
    Scene& m_scene;
    Framebuffer m_frame;
    CopyFilter m_copyFilter;

    std::vector<std::shared_ptr<BaseWidget>> m_widgets;
};

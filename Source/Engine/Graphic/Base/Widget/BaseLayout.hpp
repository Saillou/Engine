#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "BaseWidget.hpp"
#include "../Scene.hpp"
#include "../../../Utils/Filter/CopyFilter.hpp"

// -- Layout --
struct BaseLayout : 
    public BaseWidget
{
    BaseLayout(Scene& scene);
    virtual ~BaseLayout() = default;

    // Methods
    void add(std::shared_ptr<BaseWidget>);
    void add(std::shared_ptr<BaseWidget>, float x, float y);
    void clean();

    // Accessors
    Scene& scene();

    int width() const;
    int height() const;

    // Members
    float opacity = 1.0f;
    glm::vec4 background_color = glm::vec4(0,0,0,1);

protected:
    virtual void draw(Scene&) override;

private:
    virtual void _on_draw(const SceneEvents::RenderFinished&);

    Scene& m_scene;
    Framebuffer m_frame;
    CopyFilter m_copyFilter;

    std::vector<std::shared_ptr<BaseWidget>> m_widgets;
};

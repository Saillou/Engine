#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <queue>

#include "Style/StyleSheet.hpp"
#include "Widget.hpp"
#include "../Scene.hpp"
#include "../../../Utils/Filter/CopyFilter.hpp"

struct Layout : 
    public Widget
{
    // -- Layout --
    Layout(Scene& scene);
    virtual ~Layout() = default;

    // Methods
    void add(std::shared_ptr<Widget>, float x, float y);
    void clean();

    // Accessors
    Scene& scene();

    int width() const;
    int height() const;

    StyleSheet& styleSheet();

protected:
    virtual void draw(Scene&) override;

private:
    void _on_draw(const SceneEvents::RenderFinished&);

    Scene& m_scene;
    Framebuffer m_frame;
    CopyFilter m_copyFilter;
    StyleSheet m_stylesheet;

    std::vector<std::shared_ptr<Widget>> m_widgets;
};

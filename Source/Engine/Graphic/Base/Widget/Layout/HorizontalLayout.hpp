#pragma once

#include "Layout.hpp"

struct HorizontalLayout : 
    public Layout
{
    HorizontalLayout(Scene& scene, Layout::ContainerType extType = Layout::ContainerType::Free);
    virtual ~HorizontalLayout() = default;

protected:
    void draw(Scene&) override;
    glm::vec2 _compute_hull_size() override;

    Layout::ContainerType m_extType;
};

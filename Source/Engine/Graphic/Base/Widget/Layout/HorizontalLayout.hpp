#pragma once

#include "Layout.hpp"

struct HorizontalLayout : 
    public Layout
{
    HorizontalLayout(Scene& scene, Layout::ContainerType extType = Layout::ContainerType::Free);
    virtual ~HorizontalLayout() = default;

protected:
    void draw(Scene&) override;
    Layout::ContainerType m_extType;

private:
    void _computeWidgetPose();
};

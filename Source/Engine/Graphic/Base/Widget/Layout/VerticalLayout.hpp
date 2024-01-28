#pragma once

#include "Layout.hpp"

struct VerticalLayout :
    public Layout
{
    VerticalLayout(Scene& scene, Layout::ContainerType extType = Layout::ContainerType::Free);
    virtual ~VerticalLayout() = default;

protected:
    void draw(Scene&) override;
    Layout::ContainerType m_extType;

private:
    void _computeWidgetPose();
};

#include "HorizontalLayout.hpp"

HorizontalLayout::HorizontalLayout(Scene& scene, Layout::ContainerType extType) :
    Layout(scene),
    m_extType(extType)
{
    // ...
}

void HorizontalLayout::draw(Scene& scene) {
    _computeWidgetPose();
    Layout::draw(scene);
}

void HorizontalLayout::_computeWidgetPose() {
    // good luck
}



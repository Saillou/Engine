#include "VerticalLayout.hpp"

VerticalLayout::VerticalLayout(Scene& scene, Layout::ContainerType extType) :
    Layout(scene),
    m_extType(extType)
{
    // ...
}

void VerticalLayout::draw(Scene& scene) {
    _computeWidgetPose();
    Layout::draw(scene);
}

void VerticalLayout::_computeWidgetPose() {
    // good luck
}

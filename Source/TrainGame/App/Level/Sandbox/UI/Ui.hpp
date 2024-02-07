#pragma once

#include <memory>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Events/Events.hpp>
#include <Engine/Graphic/Base/SceneFrame.hpp>
#include <Engine/Graphic/Base/Widget/Text.hpp>
#include <Engine/Graphic/Base/Widget/Button.hpp>
#include <Engine/Graphic/Base/Widget/Layout/VerticalLayout.hpp>
#include <Engine/Graphic/Base/Widget/Layout/HorizontalLayout.hpp>


class Ui : Event::Subscriber 
{
public:
    Ui();
    virtual ~Ui() = default;

private:
    void createLayouts();
    StyleSheet _create_style() const;

    SceneFrame m_frame;
    std::shared_ptr<VerticalLayout> m_layoutMain;
    std::shared_ptr<VerticalLayout> m_layoutOptions;
    std::shared_ptr<VerticalLayout> m_layoutLevels;
};

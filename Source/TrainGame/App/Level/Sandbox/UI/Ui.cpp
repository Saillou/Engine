#include "Ui.hpp"

#include "Engine/Utils/Service.hpp"
#include "Engine/Graphic/Window.hpp"

Ui::Ui()
    : m_frame(Service<Window>::get().scene())
{
    createLayouts();

    m_frame.layout().add(m_layoutMain);

    m_frame.layout().styleSheet() = _create_style();
}

void Ui::createLayouts()
{
    m_layoutMain = Layout::Create<VerticalLayout>(Service<Window>::get().scene());
    m_layoutMain->add(Button::Create("Play"), "#btnPlay");
    m_layoutMain->add(Button::Create("Options"), "#btnOptions");
    m_layoutMain->add(Button::Create("Exit"), "#btnExit");
}

StyleSheet Ui::_create_style() const
{
    const glm::vec4 primary_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
    const glm::vec4 secondary_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    const glm::vec4 main_text_color = glm::vec4(0.90f, 0.90f, 0.93f, 1.0f);
    const glm::vec4 foreground_color = glm::vec4(0.66f, 0.66f, 0.70f, 1.0f);
    const glm::vec4 color_red = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);

    return StyleSheet::CreateDefault()
        .addRule(Style::Tag::Layout, Style()
            .set_opacity(0.90f)
            .set_background(primary_color)
        )
        .addRule(Style::Tag::Button, Style()
            .set_background(secondary_color)
            .set_foreground(foreground_color)
            .set_contentAlign({
                Style::Align::Centered,
                Style::Align::Centered
                })
        )
        .addRule(Style::Tag::Text, Style()
            .set_foreground(main_text_color)
        )
        .addRule("#btnOptions", Style()
            .set_background(color_red)
        )
        ;
}
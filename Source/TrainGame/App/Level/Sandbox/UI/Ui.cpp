#include "Ui.hpp"

#include <Engine/Framework/Service.hpp>
#include <Engine/Graphic/Window.hpp>

Ui::Ui()
    : m_frame(Service<Window>::get().scene())
{
    createLayouts();
    subscribeEvents();

    showPauseMenu();
    m_frame.layout().styleSheet() = _create_style();
}

void Ui::showPauseMenu()
{
    const glm::vec4 primary_color = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);

    m_frame.layout().clean();
    m_frame.layout().add(m_layoutMain);
    m_frame.layout().style().background.setValue(primary_color);
}

void Ui::createLayouts()
{
    m_layoutMain = Layout::Create<VerticalLayout>(Service<Window>::get().scene());
    m_layoutMain->add(Button::Create("Play"), "#btnPlay");
    m_layoutMain->add(Button::Create("Options"), "#btnOptions");
    m_layoutMain->add(Button::Create("Exit"), "#btnExit");
}

void Ui::subscribeEvents()
{
    _subscribe(m_layoutMain->find<Button>("#btnPlay"), [=](const WidgetEvents::MouseClick&) 
        {
            m_frame.layout().clean();
            m_frame.layout().style().background.setValue(Style::Transparent());
        });
}

StyleSheet Ui::_create_style() const
{
    const glm::vec4 primary_color = glm::vec4(0.1f, 0.1f, 0.15f, 0.7f);
    const glm::vec4 secondary_color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
    const glm::vec4 main_text_color = glm::vec4(0.90f, 0.90f, 0.93f, 1.0f);
    const glm::vec4 foreground_color = glm::vec4(0.66f, 0.66f, 0.70f, 1.0f);
    const glm::vec4 color_red = glm::vec4(0.8f, 0.3f, 0.3f, 1.0f);
    const glm::vec4 color_green = glm::vec4(0.3f, 0.8f, 0.3f, 1.0f);
    const glm::vec4 color_blue = glm::vec4(0.3f, 0.3f, 0.8f, 1.0f);

    Style s1;
    Style s2;
    Style s3;

    s1.background.setValue(color_red);

    s3 = s2 + s1;

    return StyleSheet::CreateDefault()
        .addRule(Style::Tag::Layout, Style()
            .background.setValue(primary_color)
        )
        .addRule(Style::Tag::Button, Style()
            .background.setValue(secondary_color)
            .foreground.setValue(foreground_color)
            .contentAlign.setValue({
                Style::Align::Centered,
                Style::Align::Centered
                })
            .hovered_foreground.setValue(color_green)
        )
        .addRule(Style::Tag::Text, Style()
            .foreground.setValue(main_text_color)
        )
        .addRule("#btnOptions", Style()
            .background.setValue(color_red)
            .hovered_background.setValue(color_green)
            .hovered_foreground.setValue(color_red)
        )
        .addRule("#btnExit", Style()
            .background.setValue(color_blue)
        )
        ;
}
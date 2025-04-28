#include "StatViewer.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Jauge.hpp"
#include "../Panel.hpp"

StatViewer::StatViewer(const Point& topLeft, const std::string& text, const CanvasShape::Color& filled) :
    _topLeft(topLeft),
    _color(filled)
{
    add("title", std::make_shared<Text>(
        Point{ _topLeft.x, _topLeft.y }, 0.3f,
        text,
        CanvasShape::Color(255, 255, 255, 255)
    ));

    add("jauge_stat", std::make_shared<Jauge>(
        Rectangle{ _topLeft.x + 80.0f, _topLeft.y - 5.f, 120.f, 15.f },
        _color
    ));

    add("jauge_qi", std::make_shared<Jauge>(
        Rectangle{ _topLeft.x + 80.0f, _topLeft.y + 10.f, 120.f, 5.f },
        CanvasShape::Color(255, 255, 255, 190)
    ));

    add("button_push", std::make_shared<Button>(
        Rectangle{ _topLeft.x + 210.f, _topLeft.y - 5.f, 20.f, 20.f },
        "+",
        CanvasShape::Color(255, 255, 255, 255)
    ));

    add("count_qi", std::make_shared<Text>(
        Point{ _topLeft.x + 240.f, _topLeft.y }, 0.3f,
        "0",
        CanvasShape::Color(255, 255, 255, 255)
    ));

    // Events
    _subscribe(get("button_push"), [=](const CommonEvents::MouseButton& btn) {
        if (btn.action != InputAction::Pressed || btn.button != MouseButton::Left)
            return;

        auto& ratio   = std::dynamic_pointer_cast<Jauge>(get("jauge_qi"))->ratio();
        auto& curr_qi = Panel::shared_data().idle.curr_qi;
        if (curr_qi < 1 || ratio >= 1)
            return;

        curr_qi -= 1.f;
        ratio = std::min(ratio + 0.1f, 1.f);
    });

    _subscribe(&StatViewer::_state_updated);
}

bool StatViewer::_hitArea(int x, int y) {
    return false;
}

void StatViewer::_state_updated(const CommonEvents::StateUpdated&) {
    auto& qi = std::dynamic_pointer_cast<Jauge>(get("jauge_qi"))->ratio();
    auto& stat = std::dynamic_pointer_cast<Jauge>(get("jauge_stat"))->ratio();

    float quotient = 1e-3f;
    if (qi > quotient) {
        qi -= quotient;

        if (stat < 1) {
            stat += quotient;
        }
        else {
            stat = 0;
            // +1
        }
    }
}

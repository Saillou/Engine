#pragma once

#include "PanelIdle.hpp"

#include "Elements/QiCollector.hpp"
#include "Elements/QiViewer.hpp"
#include "Elements/StatViewer.hpp"

PanelIdle::PanelIdle()
{
    // Sprites
    /* - Menu - */ 
    {
        add("button_story", std::make_shared<Button>(
            Rectangle{ width() - 200.0f, 30.0f, 100.f, 70.f },
            "Story",
            CanvasShape::Color(142, 142, 142, 255)
        ));
    }

    /* - Cultivation - */
    {
        add("text_cultivation_title", std::make_shared<Text>(
            Point{ 100.0f, 100.0f }, 0.5f,
            "",
            CanvasShape::Color(255, 255, 255, 255)
        ));
        add("qi_collector", std::make_shared<QiCollector>(
            Circle{ 100.f, 250.f, 150.0f }
        ));
        add("qi_viewer", std::make_shared<QiViewer>(
            Point{ 100.0f, 500.0f }
        ));
    }

    /* - Training - */
    {
        add("text_training_title", std::make_shared<Text>(
            Point{ width()/2 + 100.0f, 150.0f }, 0.4f,
            "Training",
            CanvasShape::Color(255, 255, 255, 255)
        ));

        add("focus_jauge", std::make_shared<StatViewer>(
            Point{ width() / 2 + 100.0f, 200.0f },
            "Focus",
            CanvasShape::Color(242, 202, 142, 200)
        ));
        add("speed_jauge", std::make_shared<StatViewer>(
            Point{ width() / 2 + 100.0f, 230.0f },
            "Speed",
            CanvasShape::Color(142, 242, 202, 200)
        ));
        add("quantity_jauge", std::make_shared<StatViewer>(
            Point{ width() / 2 + 100.0f, 260.0f },
            "Quantity",
            CanvasShape::Color(142, 202, 242, 200)
        ));
    }

    // Events
    _subscribe(get("button_story"), [=](const CommonEvents::MouseButton& btn) {
        if (btn.action == InputAction::Pressed) {
            Event::Emit(PanelIdle::Events::Back(), this);
        }
    });

    _subscribe(get("qi_collector"), [=](const QiCollector::Events::Collected& qi) {
        auto& model = shared_data().idle;
        model.curr_qi = glm::min(
            (model.base_qi + model.quantity),
            model.curr_qi + (qi.quantity * (1.0f + shared_data().idle.focus / 10.0f))
        );
        Events::Emit(Panel::Events::IdleModelUpdated());
    });

    _subscribe(get("focus_jauge"), [=](const StatViewer::Events::StatUp&) {
        std::dynamic_pointer_cast<StatViewer>(get("focus_jauge"))->stat() = (int)++shared_data().idle.focus;
        Events::Emit(Panel::Events::IdleModelUpdated());
    });

    _subscribe(get("speed_jauge"), [=](const StatViewer::Events::StatUp&) {
        std::dynamic_pointer_cast<StatViewer>(get("speed_jauge"))->stat() = (int)++shared_data().idle.speed;
        Events::Emit(Panel::Events::IdleModelUpdated());
    });

    _subscribe(get("quantity_jauge"), [=](const StatViewer::Events::StatUp&) {
        std::dynamic_pointer_cast<StatViewer>(get("quantity_jauge"))->stat() = (int)++shared_data().idle.quantity;
        Events::Emit(Panel::Events::IdleModelUpdated());
    });

    // Start
    _apply_model();
}

void PanelIdle::_apply_model() 
{
    std::dynamic_pointer_cast<Text>(get("text_cultivation_title"))->text() = 
        ModelIdle::StageToString(shared_data().idle.stage) + " (level "+std::to_string(shared_data().idle.level) + ")";

    std::dynamic_pointer_cast<StatViewer>(get("focus_jauge"))->stat()    = 
        (int)shared_data().idle.focus;

    std::dynamic_pointer_cast<StatViewer>(get("speed_jauge"))->stat()    = 
        (int)shared_data().idle.speed;

    std::dynamic_pointer_cast<StatViewer>(get("quantity_jauge"))->stat() = 
        (int)shared_data().idle.quantity;
}
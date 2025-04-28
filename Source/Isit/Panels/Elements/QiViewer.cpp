#include "QiViewer.hpp"
#include "Text.hpp"
#include "Jauge.hpp"

QiViewer::QiViewer(const Point& pt) :
    _topLeft(pt),
    _model(Panel::shared_data().idle)
{
    add("title_qi", std::make_shared<Text>(
        Point{ _topLeft.x, _topLeft.y }, 0.3f,
        "Qi",
        CanvasShape::Color(255, 255, 255, 255)
    ));

    add("jauge_qi", std::make_shared<Jauge>(
        Rectangle{ _topLeft.x + 30.0f, _topLeft.y - 5.f, 120.f, 20.f}, 
        CanvasShape::Color(255,255,255,190)
    ));

    add("count_qi", std::make_shared<Text>(
        Point{ _topLeft.x + 170.f, _topLeft.y }, 0.3f,
        "0 / 0",
        CanvasShape::Color(255, 255, 255, 255)
    ));

    _apply_model();

    // Events
    _subscribe(&QiViewer::_on_model_changed);
}

bool QiViewer::_hitArea(int x, int y) {
    return false;
}

void QiViewer::_apply_model() {
    float ratio_jauge   = _model.curr_qi / _model.base_qi;

    std::ostringstream out;
    out.precision(1);
    out << std::fixed << _model.curr_qi << " / " << _model.base_qi;

    std::dynamic_pointer_cast<Jauge>(get("jauge_qi"))->ratio() = ratio_jauge;
    std::dynamic_pointer_cast<Text>(get("count_qi"))->text() = out.str();
}

void QiViewer::_on_model_changed(const Panel::Events::IdleModelUpdated&) {
    _apply_model();
}

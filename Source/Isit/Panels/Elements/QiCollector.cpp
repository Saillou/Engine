#include "QiCollector.hpp"

namespace tag {
    constexpr char* increase_qi = "increase_qi";
    constexpr char* rotation_qi = "rotation_qi";
    constexpr char* decrease_qi = "decrease_qi";

    constexpr char* collect_qi = "collect_qi";
}

QiCollector::QiCollector(const Circle& c) :
    _model(Panel::shared_data().idle),
    _area(c)
{
    _anim_qi.add(tag::increase_qi, Animator::Tweet(0.0f, 0.7f, Animator::Tweet::Type::Quadratic));
    _anim_qi.add(tag::rotation_qi, Animator::Tweet(0.0f, 3.0f, Animator::Tweet::Type::Quadratic));
    _anim_qi.add(tag::decrease_qi, Animator::Tweet(0.0f, 0.3f, Animator::Tweet::Type::Quadratic));

    _anim_collect.add(tag::collect_qi, Animator::Tweet(0.0f, 0.15f, Animator::Tweet::Type::Linear));
    _anim_collect.add(tag::decrease_qi, Animator::Tweet(0.0f, 0.3f, Animator::Tweet::Type::Quadratic));
}

void QiCollector::draw() {
    clear();

    float max_radius = _area.r;
    float center_x = _area.x + max_radius / 2.0f;
    float center_y = _scene.height() - (_area.y + _area.r) + max_radius / 2.0f;

    float main_circle_unit = (_model.base_qi / 100.0f) * (_model.quantity + 1.f);
    float main_circle_size = 50.0f;
    float unit_circle_size = main_circle_size / main_circle_unit;
    
    // Draw radial axis
    float current_radius = unit_circle_size;
    while (current_radius <= max_radius)
    {
        float ratio_alpha = 1.2f - current_radius / max_radius;
        _canvas.get()
            .begin()
            .circle(center_x, center_y, current_radius)
            .stroke(glm::vec4(255, 255, 255, 50 * ratio_alpha), 1.7f);

        current_radius *= 2;
    }

    // Draw main circle
    _canvas.get()
        .begin()
        .circle(center_x, center_y, main_circle_size)
        .stroke(glm::vec4(255, 255, 255, 150), 2.f);

    // Draw qi
    float current_pos_angular = glm::pi<float>();
    {
        if (_anim_qi.current() == tag::rotation_qi) {
            current_pos_angular += _anim_qi.update(0.f, 2.f * glm::pi<float>());
        }
        while (current_pos_angular > 2.0f * glm::pi<float>()) {
            current_pos_angular -= 2.0f * glm::pi<float>();
        }
    }

    float size_qi = 10.0f;
    float collect_progress = 0.0f;
    if (_is_collecting) {
        if (_anim_collect.current() == tag::collect_qi) {
            collect_progress = _anim_collect.update(0.f, 1.f);
            size_qi = 10.0f;
        }
        if(_anim_collect.current() == tag::decrease_qi) {
            collect_progress = 1.f;
            size_qi = _anim_collect.update(10.f, 2.f);
        }
        if (_anim_collect.ended()) {
            collect_progress = 1.f;
            size_qi = 2.f;
        }
    }

    if (_anim_qi.current() == tag::increase_qi) {
        size_qi = _anim_qi.update(0.f, size_qi);
    }
    if (_anim_qi.current() == tag::decrease_qi) {
        size_qi = _anim_qi.update(size_qi, 0.f);
    }


    _canvas.get()
        .begin()
        .circle(
            center_x + main_circle_size * glm::cos(current_pos_angular),
            center_y + main_circle_size * glm::sin(current_pos_angular),
            size_qi
        )
        .fill(glm::vec4(255, 255, 255, 70 + (220-70) * collect_progress));

    if (_is_mouse_over()) {
        _canvas.get()
            .last()
            .stroke(glm::vec4(255, 255, 255, 120));
    }

    // Check end
    if (_anim_qi.ended()) {
        _collect_qi();
    }
}

void QiCollector::onMousePressed() {
    _is_collecting = true;
    _anim_collect.reset();
}

void QiCollector::onMouseReleased() {
    if (!_is_collecting)
        return;

    if (_anim_collect.ended()) {
        _collect_qi();
    }
    _is_collecting = false;
    _anim_collect.reset();
}

void QiCollector::onMouseOut()
{
    onMouseReleased();
}

bool QiCollector::_hitArea(int x, int y) {
    return _area.is_in(x, y);
}

void QiCollector::_collect_qi() {
    QiCollector::Events::Collected event_collected;

    if (_anim_qi.ended() && _is_collecting && !_anim_collect.ended()) {   // Perfect click
        event_collected.quantity = 2.0f;
    }
    else if (_anim_qi.ended() && _is_collecting) {                        // Click
        event_collected.quantity = 1.0f;
    }
    else if (_anim_qi.ended()){                                           // No click
        event_collected.quantity = 0.5f;
    }
    else {                                                                // Clicked out too early
        event_collected.quantity = 0.1f;
    }

    _anim_qi.reset();
    _anim_collect.reset();
    _is_collecting = false;

    // Events
    Events::Emit(event_collected, this);
}

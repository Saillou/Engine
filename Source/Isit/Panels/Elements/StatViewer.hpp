#pragma once

#include "Graphics/Sprite.hpp"
#include "Graphics/misc.hpp"
#include "../Panel.hpp"

struct StatViewer : public Sprite
{
    struct Events : public Event {
        struct StatUp : public _Base
        {
            StatUp() : _Base(_Type::EventStatUp) { }
        };

    private:
        enum _Type : int {
            EventStatUp,
        };
    };

    StatViewer(const Point& topLeft, const std::string& text, const CanvasShape::Color& filled);
    int& stat();


protected:
    virtual bool _hitArea(int x, int y) override;

private:
    void _state_updated(const CommonEvents::StateUpdated&);
    void _on_model_changed(const Panel::Events::IdleModelUpdated&);
    void _apply_model();

    Point               _topLeft;
    CanvasShape::Color  _color;
    int _current_stat = 0;
};
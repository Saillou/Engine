#pragma once

#include "Graphics/Sprite.hpp"
#include "Graphics/misc.hpp"
#include "../Panel.hpp"

struct QiViewer : public Sprite
{
    QiViewer(const Point& topLeft);

protected:
    virtual bool _hitArea(int x, int y) override;
    void _apply_model();
    void _on_model_changed(const Panel::Events::IdleModelUpdated&);

private:
    Point _topLeft;
    ModelIdle& _model;
};
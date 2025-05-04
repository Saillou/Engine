#pragma once

#include "Graphics/Sprite.hpp"
#include "Graphics/misc.hpp"
#include "../Panel.hpp"

struct QiCollector : public Sprite
{
    struct Events : public Event {
        struct Collected : public _Base 
        {
            Collected(float quantity_collected = 0.f) :
                _Base(_Type::EventCollected), 
                quantity(quantity_collected) 
            {}
            float quantity = 0.f;
        };

    private:
        enum _Type : int {
            EventCollected,
        };
    };

    QiCollector(const Circle&);
    virtual void draw() override;

protected:
    virtual void onMousePressed() override;
    virtual void onMouseReleased() override;
    virtual void onMouseOut() override;
    void _on_model_changed(const Panel::Events::IdleModelUpdated&);

    virtual bool _hitArea(int x, int y) override;

private:
    void _collect_qi();

    ModelIdle& _model;
    Circle _area;

    bool _is_collecting = false;
    Animator::Tweets _anim_qi;
    Animator::Tweets _anim_collect;
};
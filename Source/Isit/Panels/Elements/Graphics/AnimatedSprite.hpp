#pragma once

#include "../../Elements/Graphics/Sprite.hpp"
#include <Engine/Utils/Animator.hpp>

struct AnimatedSprite : public Sprite
{
    AnimatedSprite(float duration_total_sec) : 
        _tweet_total(0.f, duration_total_sec, Animator::Tweet::Type::Linear) 
    {}

    virtual ~AnimatedSprite() = default;

    void reset() {
        _tweet_total.reset();
    }

    bool ended() const {
        return _tweet_total.ended();
    }

protected:
    Animator::Tweet _tweet_total;
};
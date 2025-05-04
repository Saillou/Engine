#pragma once

#include "../../Elements/Graphics/AnimatedSprite.hpp"

#include <iostream>

namespace Animation_base
{
    static std::string Name() { 
        return "animation_base_template"; 
    }

    static float Duration() {
        return 2.0f;
    }

    struct AnimatedSprite : public ::AnimatedSprite
    {
        AnimatedSprite() : 
            ::AnimatedSprite(Duration()) {};

        void draw() override {
            clear();

            float percent = _tweet_total.update(0.f, 100.f);
            const float base_radius = 50.0f;
            float current_radius = base_radius + (percent-20.0f);

            _canvas.get()
                .begin()
                .rect(width()/2 + current_radius /2, height()/2 - current_radius /2, current_radius, current_radius)
                .fill(glm::vec4(255, 255, 255, 50 + 2* percent));
        }
    };
};
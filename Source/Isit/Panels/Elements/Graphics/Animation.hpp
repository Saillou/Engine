#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "AnimatedSprite.hpp"

struct Animation 
{
    Animation(std::shared_ptr<AnimatedSprite> internal_sprite);

    void play(std::shared_ptr<Sprite> parent_sprite);
    void stop();
    void update();
    bool ended() const;

protected:
    std::shared_ptr<Sprite> _parent = nullptr;
    std::shared_ptr<AnimatedSprite> _sprite = nullptr;
};
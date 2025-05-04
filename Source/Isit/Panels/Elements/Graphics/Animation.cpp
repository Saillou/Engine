#include "Animation.hpp"

#include <iostream>

Animation::Animation(std::shared_ptr<AnimatedSprite> internal_sprite) :
    _sprite(internal_sprite)
{
}

void Animation::play(std::shared_ptr<Sprite> parent_sprite)
{
    _parent = parent_sprite;

    if (_parent && _sprite) {
        _sprite->reset();
        _parent->add("animation", _sprite);
    }
}

void Animation::stop()
{
    if (_parent && _sprite) {
        _parent->remove("animation");
        _parent->clear();
    }

    _parent = nullptr;
}

void Animation::update()
{
    if (_sprite->ended()) {
        return;
    }

    if (_parent) {
        _parent->draw();
    }
}

bool Animation::ended() const
{
    return _sprite->ended();
}
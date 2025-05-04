#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "../Elements/Graphics/Animation.hpp"

struct AnimationManager
{
    static AnimationManager& Instance();

    void RegisterAll();
    void UnregisterAll();

    template <class SpriteAnimation>
    void Add(const std::string name) {
        static_assert(std::is_base_of<AnimatedSprite, SpriteAnimation>(), "Not an animated sprite");

        _all_animations.emplace(name, std::make_shared<Animation>(std::make_shared<SpriteAnimation>()));
    }

    void Prepare(const std::string& name);
    void Reset();
    std::shared_ptr<Animation> Current();

private:
    AnimationManager() = default;

    std::string _current;
    std::unordered_map<std::string, std::shared_ptr<Animation>> _all_animations;
};
#include "AnimationManager.hpp"

#include "Animations/Animation_base.hpp"
#include "Animations/Animation_0.hpp"

AnimationManager& AnimationManager::Instance() {
    static AnimationManager anim;
    return anim;
}

void AnimationManager::RegisterAll() {
    if (!_all_animations.empty()) // Already done
        return;

    // Define all anim possible
    AnimationManager::Add<Animation_base::AnimatedSprite>(Animation_base::Name());
    AnimationManager::Add<Animation_0::AnimatedSprite>(Animation_0::Name());
}
void AnimationManager::UnregisterAll() {
    _all_animations.clear();
}

void AnimationManager::Prepare(const std::string& name) {
    if (Current())
        Current()->stop();

    if (_all_animations.find(name) == _all_animations.cend()) {
        std::cerr << "Could not find animation: " << name << std::endl;
        _current = "";
        return;
    }

    _current = name;
}
void AnimationManager::Reset() {
    if (Current())
        Current()->stop();

    _current = "";
}
std::shared_ptr<Animation> AnimationManager::Current() {
    if (_current.empty() || _all_animations.find(_current) == _all_animations.cend()) {
        return {};
    }

    return _all_animations[_current];
}
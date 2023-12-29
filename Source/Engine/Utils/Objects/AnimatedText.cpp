#include "AnimatedText.hpp"

#include "../../Graphic/TextEngine.hpp"

// Utils
glm::vec2 AnimatedText::s_viewport_size = glm::vec2(800, 600);

AnimatedText::AnimatedText(const Data& start, const Data& end, float start_offset, float duration, Animator::Tweet::Type type) :
    _start(start),
    _end(end),
    _current(start),
    _tweet(start_offset, duration, type)
{
    // ..
}

void AnimatedText::draw() {
    _current.pos = _tweet.update(_start.pos, _end.pos);
    _current.size = _tweet.update(_start.size, _end.size);
    _current.color = _tweet.update(_start.color, _end.color);
    _current.text = _tweet.update(_start.text, _end.text);

    TextEngine::Write(
        _current.text,
        s_viewport_size.x * _current.pos.x, s_viewport_size.y * _current.pos.y,
        _current.size,
        _current.color
    );
}


// Helper functions
uAText FadeIn(const ATextD& state, float start_time, float duration) {
    return std::make_shared<AnimatedText>
        (
            ATextD{ state.text, state.pos, state.size, glm::vec3(0.05f, 0.05f, 0.06f) },
            state,
            start_time, duration,
            Animator::Tweet::Type::Quadratic
        );
};

uAText FadeOut(const ATextD& state, float start_time, float duration) {
    return std::make_shared<AnimatedText>
        (
            state,
            ATextD{ state.text, state.pos, state.size, glm::vec3(0.05f, 0.05f, 0.06f) },
            start_time, duration,
            Animator::Tweet::Type::Quadratic
        );
};

uAText SlideTop(const ATextD& state, float start_time, float duration) {
    return std::make_shared<AnimatedText>
        (
            state,
            ATextD{ state.text, glm::vec2(state.pos.x, state.pos.y + 2.0f), state.size, state.color },
            start_time, duration,
            Animator::Tweet::Type::Linear
        );
};
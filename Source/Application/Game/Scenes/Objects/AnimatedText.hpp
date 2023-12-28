#pragma once

#include <glm/glm.hpp>
#include <memory>

#include <Engine/Utils/Timeline.hpp>
#include <Engine/Utils/Animator.hpp>

// -- Utils --
struct Drawable {
    virtual ~Drawable() = default;
    virtual void draw() = 0;
};

struct AnimatedText : public Drawable
{
    // Statics
    static glm::vec2 s_viewport_size;

    // Subs
    struct Data {
        std::string text;
        glm::vec2 pos;    // [0, 1] x [0, 1]
        float size;
        glm::vec3 color;
    };

    // Methods
    AnimatedText(const Data& start, const Data& end, float start_offset, float duration, Animator::Tweet::Type type = Animator::Tweet::Type::Linear);
    void draw() override;

private:
    const Data _start;
    const Data _end;
    Data _current;
    Animator::Tweet _tweet;
};

// I'm lazy
typedef AnimatedText::Data ATextD;
typedef std::shared_ptr<AnimatedText> uAText;

// - Helpers -
uAText FadeIn(const ATextD& state, float start_time, float duration);
uAText FadeOut(const ATextD& state, float start_time, float duration);
uAText SlideTop(const ATextD& state, float start_time, float duration);
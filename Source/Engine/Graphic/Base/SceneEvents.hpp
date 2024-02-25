#pragma once

#include "../../Events/Events.hpp"

// Define events available
struct SceneEvents : public Event {
    enum _Type {
        RenderStarted_ = 1024,
        PostDraw_,
        RenderFinished_,
        Resized_,
    };

    // -- Inputs --
    struct RenderStarted : public _Base {
        RenderStarted() :
            _Base(_Type::RenderStarted_)
        {};
    };
    struct PostDraw : public _Base {
        PostDraw():
            _Base(_Type::PostDraw_)
        {};
    };
    struct Resized : public _Base {
        Resized(int width = 0, int height = 0):
            _Base(_Type::Resized_), width(width), height(height)
        {};

        int width;
        int height;
    };
    struct RenderFinished : public _Base {
        RenderFinished() :
            _Base(_Type::RenderFinished_)
        {};
    };
};



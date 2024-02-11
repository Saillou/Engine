#pragma once

#include "../../Events/Events.hpp"

// Define events available
struct SceneEvents : public Event {
    enum _Type {
        Draw_ = 1024,
        PreDraw_,
        PostDraw_,
        RenderFinished_,
        SceneFinished_,
        Resized_,
    };

    // -- Inputs --
    struct Draw : public _Base {
        Draw():
            _Base(_Type::Draw_)
        {};
    };
    struct PreDraw : public _Base {
        PreDraw() :
            _Base(_Type::PreDraw_)
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
    struct SceneFinished : public _Base {
        SceneFinished() :
            _Base(_Type::SceneFinished_)
        {};
    };
};

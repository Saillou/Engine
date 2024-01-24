#pragma once

#include "../../../Events/Events.hpp"

// Define events available
struct LayoutEvents : public Event {
    enum _Type {
        LayoutDraw_ = 2048,
    };

    // -- Inputs --
    struct Draw : public _Base {
        Draw():
            _Base(_Type::LayoutDraw_)
        {};
    };
};

#pragma once

#include "../../../../Events/Events.hpp"

// Define events available
struct WidgetEvents : public Event {
    enum _Type {
        Draw_ = 2048,
        MouseClick_,
        MouseOver_,
        MouseOut_,
    };

    // -- Inputs --
    struct Draw : public _Base { 
        Draw(): _Base(_Type::Draw_) {}; 
    };

    struct MouseClick : public _Base {
        MouseClick() : _Base(_Type::MouseClick_) {};
    };

    struct MouseOver : public _Base {
        MouseOver() : _Base(_Type::MouseOver_) {};
    };

    struct MouseOut : public _Base {
        MouseOut() : _Base(_Type::MouseOut_) {};
    };
};

#pragma once

#include <unordered_map>
#include "Panel.hpp"

struct PanelIdle : public Panel
{
    struct Events : public Event
    {
        struct Back : public _Base
        {
            Back() : _Base(_Type::EventBack) {}
        };

    private:
        enum _Type : int {
            EventBack,
        };
    };

    PanelIdle();

private:
    void _apply_model();
};
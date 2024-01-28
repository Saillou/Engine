#pragma once

#include "Widget.hpp"
#include "Text.hpp"

struct Button : public Widget {
    Button(const std::string& text);

    // Methods
    void draw(Scene&) override;

    // Members
    Text content;

    float padding = 0.05f; // todo:  be put in style
};

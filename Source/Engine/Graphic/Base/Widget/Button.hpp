#pragma once

#include "Widget.hpp"
#include "Text.hpp"

struct Button : public Widget {
    Button(const std::string& text);

    // Methods
    void draw(Scene&) override;

    // Members
    Text content;

private:
};

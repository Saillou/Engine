#pragma once

#include "Widget.hpp"

struct Button : public Widget {
    Button(const std::string& text);

    // Methods
    void draw(Scene&) override;

    // Members
    float& w();
    float& h();

    std::string text;

    glm::vec4 backgroundColor;
    glm::vec4 foregroundColor;

};

#pragma once

#include "BaseWidget.hpp"

struct ButtonWidget : public BaseWidget {
    ButtonWidget(const std::string& text);

    void draw(Scene&) override;

    float& w();
    float& h();

    std::string text;

    glm::vec4 backgroundColor;
    glm::vec4 foregroundColor;
};
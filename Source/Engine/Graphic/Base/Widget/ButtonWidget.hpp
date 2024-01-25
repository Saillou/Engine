#pragma once

#include "BaseWidget.hpp"

struct ButtonWidget : public BaseWidget {
    ButtonWidget(const std::string& text);

    void draw(Scene&) override;
    std::string& text();

    float& w();
    float& h();

private:
    std::string _text;

    glm::vec4 _backgroundColor;
    glm::vec4 _foregroundColor;
};
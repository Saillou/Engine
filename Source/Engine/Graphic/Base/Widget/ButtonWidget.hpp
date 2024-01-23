#pragma once

#include "BaseWidget.hpp"

struct ButtonWidget {
    ButtonWidget();

    void draw(Scene&);

private:
    Quad _surface;
    const std::string _text;

    glm::vec4 _backgroundColor;
    glm::vec4 _foregroundColor;
};
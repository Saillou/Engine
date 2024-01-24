#pragma once

#include <memory>
#include "BaseLayout.hpp"

struct ButtonWidget : public BaseWidget {
    ButtonWidget(const std::string& text);

    void draw(Scene&) override;

private:

    Quad _surface;
    const std::string _text;

    glm::vec4 _backgroundColor;
    glm::vec4 _foregroundColor;
};
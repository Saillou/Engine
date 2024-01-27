#pragma once

#include <glm/glm.hpp>

struct Style {
    virtual ~Style() = default;

    // Categories
    enum class Tag {
        None = 0,
        Layout, Button, Text,
        Custom = 400
    };

    // Constantes
    static const float Unset();
    static const glm::vec4 Transparent();

    // Position 
    enum class VerticalAlign {
        Free = 0, 
        Left, Right, Centered
    };
    enum class HorizontalAlign {
        Free = 0, 
        Top, Bottom, Centered
    };

    // Helper
    Style operator+(const Style& styleNew);

    // Members
    float hint_x         = Unset();
    float hint_y         = Unset();
    float hint_w         = Unset();
    float hint_h         = Unset();

    float opacity        = 1.0f;
    float textSize       = 0.5f;
    glm::vec4 background = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 foreground = glm::vec4(1.0f);
    glm::vec4 borders    = Transparent();

    struct Align {
        VerticalAlign   v;   
        HorizontalAlign h;
    } contentAlign = { 
        VerticalAlign::Free, 
        HorizontalAlign::Free 
    };
};
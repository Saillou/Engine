#pragma once

#include <string>
#include <optional>
#include <glm/glm.hpp>

#include "StyleItem.hpp"

// Note: If you wonder wtf with the optional and weird accessors:
//  it's not really a troll, 
// i'm trying to:
//  - have a 'trace' (or 'clean/dirty') information before any modification happens on a member value..
//  - ..while also having a default value if asked..
//  - ..and chaining the set.


struct Style {
    Style();
    static Style CreateBasic();

    std::string test;

    // Categories
    enum class Tag {
        None = 0,
        Layout, Button, Text,
        Custom = 400
    };

    // Constantes
    static const glm::vec4 Transparent();

    // Position 
    enum class Align {
        Free = 0, 
        Start, End, Centered
    };
    struct BoxAlign {
        Align v;
        Align h;
    };

    // Helper
    Style operator+(const Style& styleNew) const;

    StyleItem<float> hint_x;
    StyleItem<float> hint_y;
    StyleItem<float> hint_w;
    StyleItem<float> hint_h;
    StyleItem<float> opacity;
    StyleItem<float> textSize;
    StyleItem<glm::vec4> background;
    StyleItem<glm::vec4> foreground;
    StyleItem<glm::vec4> borders;
    StyleItem<BoxAlign>  contentAlign;
    StyleItem<glm::vec4> hovered_background;
    StyleItem<glm::vec4> hovered_foreground;
    StyleItem<glm::vec4> clicked_background;
    StyleItem<glm::vec4> clicked_foreground;

private:

};
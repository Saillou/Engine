#pragma once

#include <optional>
#include <glm/glm.hpp>

// Note: If you wonder wtf with the optional and weird accessors:
//  it's not really a troll, 
// i'm trying to:
//  - have a 'trace' (or 'clean/dirty') information before any modification happens on a member value..
//  - ..while also having a default value if asked..
//  - ..and chaining the set.


struct Style {
    Style() = default;
    static Style CreateBasic();

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

    // Accessors
    float     hint_x()       const;
    float     hint_y()       const;
    float     hint_w()       const;
    float     hint_h()       const;
    float     opacity()      const;
    float     textSize()     const;
    glm::vec4 background()   const;
    glm::vec4 foreground()   const;
    glm::vec4 borders()      const;
    BoxAlign  contentAlign() const;

    bool has_hint_x()       const;
    bool has_hint_y()       const;
    bool has_hint_w()       const;
    bool has_hint_h()       const;
    bool has_opacity()      const;
    bool has_textSize()     const;
    bool has_background()   const;
    bool has_foreground()   const;
    bool has_borders()      const;
    bool has_contentAlign() const;

    Style& set_hint_x(float);
    Style& set_hint_y(float);
    Style& set_hint_w(float);
    Style& set_hint_h(float);
    Style& set_opacity(float);
    Style& set_textSize(float);
    Style& set_background(const glm::vec4&);
    Style& set_foreground(const glm::vec4&);
    Style& set_borders(const glm::vec4&);
    Style& set_contentAlign(const BoxAlign&);

private:
    static Style _s_basic_style;

    // Members
    std::optional<float> _hint_x;
    std::optional<float> _hint_y;
    std::optional<float> _hint_w;
    std::optional<float> _hint_h;
    std::optional<float> _opacity;
    std::optional<float> _textSize;
    std::optional<glm::vec4> _background;
    std::optional<glm::vec4> _foreground;
    std::optional<glm::vec4> _borders;
    std::optional<BoxAlign>  _contentAlign;

    template<typename T>
    static std::optional<T> _Select(std::optional<T> base, std::optional<T> ope) {
        if (!ope)
            return base;
        return ope;
    }
};
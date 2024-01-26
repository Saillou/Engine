#pragma once

#include "Widget.hpp"

struct Text : public Widget {
    typedef std::vector<std::string> Block;

    Text(const std::string& text, float scale = 0.5f);
    Text(const Block& texts, float scale = 0.5f);

    // Methods
    void draw(Scene&) override;

    // Accessors
    const std::string& getText() const;
    const Block& getTexts() const;

    const glm::vec2& getSize() const;
    const glm::vec2 getSizeRel() const;

    void setText(const std::string& txt);
    void setTexts(const Block& txt);

    std::string& at(size_t i);
    std::string& operator[](size_t i);

    // Member
    glm::vec4 foregroundColor;

private:
    float _scale;
    Block _texts;
    glm::vec2 _size;

    void _compute_size();
};
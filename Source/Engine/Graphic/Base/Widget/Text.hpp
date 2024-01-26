#pragma once

#include "Widget.hpp"

struct Text : public Widget {
    Text(const std::string& text, float scale = 0.5f);
    Text(const std::vector<std::string>& texts, float scale = 0.5f);

    // Methods
    void draw(Scene&) override;

    // Accessors
    const std::string& getText() const;
    const std::vector<std::string>& getTexts() const;

    const glm::vec2& getSize() const;

    void setText(const std::string& txt);
    void setTexts(const std::vector<std::string>& txt);

    std::string& at(size_t i);
    std::string& operator[](size_t i);

    // Member
    glm::vec4 foregroundColor;

private:
    float _scale;
    std::vector<std::string> _texts;
    glm::vec2 _size;
};
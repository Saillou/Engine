#pragma once

#include "Widget.hpp"

struct Text : public Widget {
    // Paragraph helper
    struct Block : std::vector<std::string> {
        Block() = default;
        Block(const std::string& data);
        Block(const char* data);
        Block(const std::initializer_list<std::string>& data);
    };

    // Text widget
    Text(const Block& texts, float textSize = -1.f);

    // Methods
    void draw(Scene&) override;

    // Accessors
    const std::string&  getText()  const;
    const Block&        getTexts() const;
    const glm::vec2&    getSize()  const;

    void setText(const std::string& txt);
    void setTexts(const Block& txt);

    std::string& at(size_t i);
    std::string& operator[](size_t i);

private:
    Block _texts;
    glm::vec2 _size;

    void _compute_size();
};
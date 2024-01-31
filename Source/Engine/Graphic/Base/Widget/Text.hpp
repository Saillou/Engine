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

    template<typename ...Args>
    static std::shared_ptr<Text> Create(Args&... args) {
        return std::make_shared<Text>(args...);
    }

    template<const char* classname>
    struct WithClass {
        template<typename ...Args>
        static std::shared_ptr<Text> Create(Args&... args) {
            auto widget = std::make_shared<Text>(args...);
            widget->className = classname;

            return widget;
        }
    };

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

    float padding = 0.02f; // todo:  be put in style

private:
    Block _texts;
    glm::vec2 _size;

    void _compute_size();
};
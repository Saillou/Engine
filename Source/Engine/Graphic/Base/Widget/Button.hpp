#pragma once

#include "Widget.hpp"
#include "Text.hpp"

struct Button : public Widget {
    Button(const std::string& text);

    template<typename ...Args>
    static std::shared_ptr<Button> Create(Args&... args) {
        return std::make_shared<Button>(args...);
    }

    template<const char* classname>
    struct WithClass
    {
        template<typename ...Args>
        static std::shared_ptr<Button> Create(Args&... args) {
            auto widget = std::make_shared<Button>(args...);
            widget->className = classname;

            return widget;
        }
    };

    // Methods
    void draw(Scene&) override;

    // Members
    Text content;

    float padding = 0.05f; // todo:  be put in style
};

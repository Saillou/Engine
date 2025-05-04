#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include <optional>

namespace StoryEngine 
{
    struct Element {
        using Condition = std::function<bool()>;
        using Action = std::function<void()>;
        struct Target {
            enum class type {
                None, Page, Panel
            };

            type type;
            std::string id;
        };

        Element(const Action& aAction);
        Element(const char* aContent);
        Element(const char* aContent, const Action& aAction);
        Element(const char* aContent, const Target& aTarget);
        Element(const char* aContent, const Target& aTarget, const Action& aAction);
        Element(const char* aContent, const Condition& aCondition);
        Element(const char* aContent, const Condition& aCondition, const Action& aAction);
        Element(const char* aContent, const Condition& aCondition, const Target& aTarget);
        Element(const char* aContent, const Condition& aCondition, const Target& aTarget, const Action& aAction);

        std::string content;
        Target target = { Target::type::None };
        Condition condition;
        Action action;
    };

    struct IO {
        enum class tag {
            Input, Output
        };

        IO(tag type_, const std::vector<Element>& elements_);
        IO(tag type_, const Element& element_);

        tag type;
        std::vector<Element> elements;
    };
    struct Node
    {
        std::unordered_map<std::string, std::string> variables;
        std::vector<IO> ios;

        IO& current_io();
        bool next();
        void reset();

    private:
        size_t _i_io = 0;
    };
    
    using Story = std::unordered_map<std::string, Node>;
};
#pragma once

#include <optional>
#include <unordered_set>
#include <unordered_map>

#include "../Widget.hpp"

struct StyleSheet 
{
    static StyleSheet CreateDefault();

    virtual ~StyleSheet() = default;

    std::optional<Style> getStyle(const Widget*);
    void applyStyle(Widget*);

    void addRule(int, const Style& style);
    void addRule(const std::string&, const Style& style);

protected:
    std::unordered_map<std::string, Style> m_class_styles;
    std::unordered_map<int,         Style> m_tag_styles;
};
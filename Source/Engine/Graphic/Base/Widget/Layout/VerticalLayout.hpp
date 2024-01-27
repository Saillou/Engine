#pragma once

#include "Layout.hpp"

struct VerticalLayout :
    public Layout
{
    VerticalLayout(Scene& scene);
    virtual ~VerticalLayout() = default;

    // Methods
    void add(std::shared_ptr<Widget>, const std::string& id = "") override;

private:

};

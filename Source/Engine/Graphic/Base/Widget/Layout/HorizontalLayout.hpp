#pragma once

#include "Layout.hpp"

struct HorizontalLayout : 
    public Layout
{
    HorizontalLayout(Scene& scene);
    virtual ~HorizontalLayout() = default;

    // Methods
    void add(std::shared_ptr<Widget>, const std::string& id = "") override;

private:

};

#pragma once

#include <string>
#include <vector>

#include "../Render/RenderType.hpp"
#include "Model.hpp"
#include "Material.hpp"

struct Entity
{
    enum SimpleShape {
        Custom, Cube, Sphere
    };

    Entity(SimpleShape shape = SimpleShape::Custom);
    Entity(const std::string& path);

    virtual ~Entity() = default;

    // Getters
    Model model;
    Material material;
};

#pragma once

#include <string>
#include <vector>

#include "../../Light.hpp"
#include "../../Camera.hpp"
#include "../../Cookable.hpp"
#include "Model.hpp"

struct Entity : public Cookable 
{
    enum SimpleShape {
        Custom, Cube, Sphere
    };

    Entity(SimpleShape shape = SimpleShape::Custom);
    Entity(const std::string& path);

    virtual ~Entity() = default;

    // Methods
    void drawOne(Cookable::CookType, const Camera&, const glm::mat4& quat = glm::mat4(1.0f), const std::vector<Light>& = {});

    void drawBatch   (const Camera&, const std::vector<Light>& = {});
    void drawShadow  (const Camera&, const Light&);
    void drawGeometry(const Camera&);

    // Getters
    Model model;
};

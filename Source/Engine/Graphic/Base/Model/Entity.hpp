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
    void draw     (const Camera& camera, const glm::mat4& model = glm::mat4(1.0f), const std::vector<Light>& = {});
    void drawBatch(const Camera& camera);

    void drawElements(Shader& shader);

    // Setters
    void setBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models);

    // Getters
    const Model& model() const;

 protected:
    Model m_model;
};

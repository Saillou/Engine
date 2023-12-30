#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "../BaseShape.hpp"
#include "../Cookable.hpp"
#include "../Light.hpp"
#include "../../Camera.hpp"

// Objects
struct Box : 
    public BaseShape,
    public Cookable
{
    // Instance
    Box(float size);
    Box(const glm::vec3& dims);
    virtual ~Box() = default;

    void draw(const Camera& camera, const glm::mat4& quad, const std::vector<Light>& lights = {});
    void draw(const Camera& camera, const glm::vec3& position = {}, const glm::vec3& orientation = {}, const std::vector<Light>& lights = {});
    void drawBatch(size_t amount, const Camera& camera);

private:
    void _setupMesh(const glm::vec3& dims);
};
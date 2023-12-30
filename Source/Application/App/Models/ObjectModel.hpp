#pragma once

#include <string>

#include <Engine/Graphic/Camera.hpp>
#include <Engine/Graphic/Model/Model.hpp>

struct ObjectModel {
    ObjectModel(const std::string& path);

    void draw(const Camera& camera, const glm::mat4& model = glm::mat4(1.0f));

private:
    Shader _shader;
    Model _object;
};

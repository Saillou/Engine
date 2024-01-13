#pragma once

#include <string>
#include <vector>

#include "../../Light.hpp"
#include "../../Camera.hpp"
#include "../../Cookable.hpp"
#include "Model.hpp"

struct ObjectModel : private Cookable {
    ObjectModel(const std::string& path);

    void draw(const Camera& camera, const glm::mat4& model = glm::mat4(1.0f), const std::vector<Light>& = {});
    void drawElements(Shader& shader);

private:
    Model _object;
};

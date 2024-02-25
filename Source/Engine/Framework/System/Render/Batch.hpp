#pragma once

#include "../../../Graphic/Base/Model/Model.hpp"
#include "../../Core/ECSTypes.hpp"

#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>

struct _Batch {
	std::vector<Entity>     _entities;
	std::vector<glm::mat4>	_transforms;
	std::vector<glm::vec4>	_materials;
};

typedef std::unordered_map<Model::Ref, _Batch> Batch;
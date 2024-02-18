#pragma once

#include "../../Graphic/Base/Model/Model.hpp"


struct RenderComponent 
{
	enum RenderType 
	{
		Hidden = 0,
		Particle,
		Basic,
		Lights,
		Shadows,
		Geometry,
		Custom = 2048
	};

	RenderType type  = RenderType::Hidden;
	Model::Ref model = nullptr;

	std::vector<glm::mat4> transforms = {};
	std::vector<glm::vec4> materials  = {};
};
#pragma once

#include <glm/glm.hpp>

struct CastComponent 
{
	bool is_hit = false;

	glm::vec3 hit_position;
	float	  hit_distance;
};
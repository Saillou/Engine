#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "../Core/ECSTypes.hpp"

struct CollideComponent 
{
	enum HitTag
	{
		Disabled = -1,
		Hitbox   = 0,
		Accurate = (1 << 0),
		Multiple = (1 << 1),
	}; 
	typedef int CollideType;

	// Data
	CollideType type  = Hitbox;
	bool is_colliding = false;

	std::vector<Entity>		hit_entities  = {};
	std::vector<glm::vec3>	hit_positions = {};
};
#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

struct CanvasShape
{
	typedef glm::vec4 Color;
	typedef glm::vec2 Point;
	typedef std::vector<Point> Points;

	// Members
	enum class DrawType {
		Fill,	// Fill content 
		Stroke	// Draw contours
	} type = DrawType::Fill;

	Color  color  = Color(1.0f);
	Points points = {};
};
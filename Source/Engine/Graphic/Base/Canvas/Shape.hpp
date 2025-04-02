#pragma once

#include <vector>
#include <memory>
#include <string>
#include <variant>
#include <glm/glm.hpp>

struct CanvasShape
{
	typedef glm::vec4 Color;
	typedef glm::vec2 Point;
	typedef std::vector<Point> Points;

	// Members
	enum class ShapeType 
	{
		None,
		Shape,
		Text
	} shapeType = ShapeType::None;

	// --- Data ------
	struct DataShape 
	{
		Points points	= {};
	} dataShape;

	struct DataText
	{
		Point start			= {};
		std::string text	= "";
		float fontSize		= 1.0f;
	} dataText;
	// ------------------

	enum class DrawType 
	{
		Fill,	// Fill content 
		Stroke	// Draw contours
	} drawType = DrawType::Fill;

	Color color		= Color(1.0f);
	float thickness = 1.0f;

	// Method
	void clear() 
	{
		if (shapeType == ShapeType::Shape) {
			dataShape.points.clear();
		}
		if (shapeType == ShapeType::Text) {
			dataText.text.clear();
		}

		shapeType = ShapeType::None;
	}
};
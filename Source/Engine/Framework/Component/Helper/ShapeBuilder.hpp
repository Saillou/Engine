#pragma once

#include "../../../Graphic/Base/Canvas/Shape.hpp"

// It's a bad idea to construct this object, use CanvasComponent helper instead.

struct ShapeBuilder 
{
	friend struct CanvasComponent;

	// Chained methods
	ShapeBuilder& rect(float x, float y, float w, float h);
	ShapeBuilder& circle(float x, float y, float r);
	ShapeBuilder& moveTo(float x, float y);
	ShapeBuilder& lineTo(float x, float y);
	ShapeBuilder& fill(const CanvasShape::Color& color);
	ShapeBuilder& stroke(const CanvasShape::Color& color, float thickness);

private:
	static std::shared_ptr<ShapeBuilder> _s_instance;
	static ShapeBuilder& Start(std::vector<CanvasShape>* shapes);
	static void Reset();

	CanvasShape _current_shape;
	std::vector<CanvasShape>* _current_container = nullptr;
};
#pragma once

#include "Helper/ShapeBuilder.hpp"
#include "../../Graphic/Base/Canvas/Shape.hpp"
#include "../../Graphic/Base/Canvas/Context.hpp"

struct CanvasComponent
{
	// - Members -
	CanvasContext context;
	std::vector<CanvasShape> shapes;

	// - Helpers -
	void clear() {
		shapes.clear();
	}

	CanvasContext& ctx() {
		return context;
	}

	ShapeBuilder& begin() {
		return ShapeBuilder::Start(&shapes);
	}
};
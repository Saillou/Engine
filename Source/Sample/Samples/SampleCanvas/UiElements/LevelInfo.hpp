#pragma once

#include "UiElements.hpp"

struct LevelInfo : public UiElements {
	LevelInfo():
		_x(0.0f), _y(0.0f)
	{
		canvas()
			.ctx()
			.dimensions(CanvasContext::Dimensions::Relative)
			.colors(CanvasContext::ColorFormat::Uint8);

		_update();
	}

	void set_position(glm::vec2& bubble_position) {
		_x = bubble_position.x;
		_y = bubble_position.y;

		_update();
	}

private:
	void _update() {
		canvas()
			.clear();

		canvas()
			.begin()
			.rect(_x, _y, 0.5f, 0.5f)
			.fill(glm::vec4(42, 42, 45, 255))
			.stroke(glm::vec4(182, 182, 185, 127), 2.0f);
	}

	float _x;
	float _y;
};
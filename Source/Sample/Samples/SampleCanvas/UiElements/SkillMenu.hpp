#pragma once

#include "UiElements.hpp"

struct SkillMenu : public UiElements {
	SkillMenu() 
	{
		canvas()
			.ctx()
			.dimensions(CanvasContext::Dimensions::Absolute)
			.colors(CanvasContext::ColorFormat::Uint8);

		canvas()
			.begin()
			.circle(125,125,25)
			.fill(glm::vec4(42, 170, 180, 255))
			.stroke(glm::vec4(182, 250, 255, 127), 2.0f);
	}
};
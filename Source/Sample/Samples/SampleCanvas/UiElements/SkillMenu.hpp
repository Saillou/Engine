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
			.rect(100, 100, 150, 150)
			.fill(glm::vec4(42, 42, 45, 255))
			//.stroke(glm::vec4(0, 0, 0, 255))
		;

		canvas()
			.begin()
			.moveTo(150, 150)
			.lineTo(200, 150)
			.lineTo(150, 200)
			.fill(glm::vec4(142, 42, 45, 255))
		//	.stroke(glm::vec4(0, 0, 0, 255))
		;
	}
};
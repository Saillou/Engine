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
			.stroke(glm::vec4(255, 255, 255, 255))
		;

		canvas()
			.begin()
			.circle(125,125,25)
			.fill(glm::vec4(42, 170, 180, 255))
			.stroke(glm::vec4(255, 255, 255, 255))
		;

		canvas()
			.begin()
			.moveTo(250, 250)
			.lineTo(300, 300)
			.lineTo(450, 200)
			.fill(glm::vec4(142, 42, 45, 255))
			.stroke(glm::vec4(255, 255, 255, 255))
		;
	}
};
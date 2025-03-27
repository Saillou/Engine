#pragma once

#include "UiElements.hpp"

struct TextBubble : public UiElements {
	TextBubble() 
	{
		canvas()
			.ctx()
			.dimensions(CanvasContext::Dimensions::Absolute)
			.colors(CanvasContext::ColorFormat::Uint8);

		canvas()
			.begin()
			.moveTo(250, 250)
			.lineTo(300, 300)
			.lineTo(450, 200)
			.fill(glm::vec4(142, 42, 45, 255))
			.stroke(glm::vec4(255, 182, 185, 127), 2.0f);
	}
};
#pragma once

struct CanvasContext
{
	// - Members -
	enum class Dimensions {
		Absolute, // Use pixels
		Relative  // Use percentages
	} dimensionsFormat = Dimensions::Relative;

	enum class ColorFormat {
		Uint8,	 // [0 - 255]
		Float	 // [0.0f - 1.0f]
	} colorFormat = ColorFormat::Float;

	// - Chained setters -
	CanvasContext& dimensions(Dimensions dim) {
		dimensionsFormat = dim;
		return *this;
	}

	CanvasContext& colors(ColorFormat fmt) {
		colorFormat = fmt;
		return *this;
	}
};
#pragma once

#include "ShapeBuilder.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Statics
std::shared_ptr<ShapeBuilder> ShapeBuilder::_s_instance = std::make_shared<ShapeBuilder>();

ShapeBuilder& ShapeBuilder::Start(std::vector<CanvasShape>* shapes) 
{
	Reset();

	_s_instance->_current_container = shapes;
	return *_s_instance;
}

void ShapeBuilder::Reset()
{
	_s_instance->_current_shape = CanvasShape();
	_s_instance->_current_container = nullptr;
}

// Instance
ShapeBuilder& ShapeBuilder::rect(float x, float y, float w, float h) 
{
	assert(_current_container && "No container");

	_current_shape.points.push_back(CanvasShape::Point(x,   y));
	_current_shape.points.push_back(CanvasShape::Point(x+w, y));
	_current_shape.points.push_back(CanvasShape::Point(x+w, y+h));
	_current_shape.points.push_back(CanvasShape::Point(x,   y+h));
	return *this;
}
ShapeBuilder& ShapeBuilder::circle(float x, float y, float r)
{
	assert(_current_container && "No container");

	_current_shape.points.push_back(CanvasShape::Point(x, y));
	for (int i = 0; i <= 50; i++) {
		float theta = i * glm::two_pi<float>() / 50.0f;
		_current_shape.points.push_back(CanvasShape::Point(x + r * glm::cos(theta), y + r * glm::sin(theta)));
	}
	return *this;
}
ShapeBuilder& ShapeBuilder::moveTo(float x, float y) 
{
	assert(_current_container && "No container");

	_current_shape.points.push_back(CanvasShape::Point(x, y));
	return *this;
}
ShapeBuilder& ShapeBuilder::lineTo(float x, float y) 
{
	assert(_current_container && "No container");

	_current_shape.points.push_back(CanvasShape::Point(x, y));
	return *this;
}

ShapeBuilder& ShapeBuilder::fill(const CanvasShape::Color& color) 
{
	assert(_current_container && "No container");

	_current_shape.type = CanvasShape::DrawType::Fill;
	_current_shape.color = color;
	_current_container->push_back(_current_shape);
	return *this;
}
ShapeBuilder& ShapeBuilder::stroke(const CanvasShape::Color& color) 
{
	assert(_current_container && "No container");

	_current_shape.type = CanvasShape::DrawType::Stroke;
	_current_shape.color = color;
	_current_container->push_back(_current_shape);
	return *this;
}
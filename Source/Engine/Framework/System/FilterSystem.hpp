#pragma once

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/FilterComponent.hpp"
#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Wrapper/Framebuffer.hpp"


struct FilterSystem : 
	public System 
{
	FilterSystem(const Camera& camera);
	~FilterSystem() = default;

	void init() override;

	void start();
	void apply();

private:
	void _drawFrame();

	// Members
	const Camera& _camera;	// Given by Scene

	Framebuffer _framebuffer_main;
	Framebuffer _framebuffer_filter;
	Quad _screenQuad;
};

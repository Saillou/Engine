#include "RenderSystem.hpp"
#include <iostream>

RenderSystem::RenderSystem(Camera& camera, std::vector<Light>& lights):
	_camera(camera), 
	_lights(lights) 
{
	std::cout << "Create render system" << std::endl;
}

RenderSystem::~RenderSystem()
{
	std::cout << "Destroy render system" << std::endl;
}

void RenderSystem::update()
{
	std::cout << "Update render system: " << _camera.screenSize.x << " " << _camera.screenSize.y << std::endl;
}

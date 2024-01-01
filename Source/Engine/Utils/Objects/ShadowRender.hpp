#pragma once

#include <functional>

#include "../../Graphic/Shaders/Shader.hpp"
#include "../../Graphic/Utils/Framebuffer.hpp"
#include "../../Graphic/Camera.hpp"

struct ShadowRender 
{
	ShadowRender();

	void bindTexture();
	void render(const Camera& camera, const glm::vec3& lightpos, std::function<void(Shader&)> renderFunc);

private:
	Framebuffer fbo;
	Shader shader;
};
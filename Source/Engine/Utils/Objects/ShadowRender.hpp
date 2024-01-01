#pragma once

#include <functional>

#include "../../Graphic/Shaders/Shader.hpp"
#include "../../Graphic/Utils/Framebuffer.hpp"
#include "../../Graphic/Base/Light.hpp"
#include "../../Graphic/Camera.hpp"

struct ShadowRender 
{
	ShadowRender();

	void bindTexture(GLuint target = GL_TEXTURE1);
	void render(const Camera& camera, const Light& light, std::function<void(Shader&)> renderFunc);

private:
	Framebuffer fbo;
	Shader shader;
};
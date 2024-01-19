#pragma once

#include <vector>
#include <functional>

#include "../../Graphic/Utils/Shader.hpp"
#include "../../Graphic/Utils/Framebuffer.hpp"
#include "../../Graphic/Light.hpp"
#include "../../Graphic/Camera.hpp"

struct ShadowRender 
{
	ShadowRender();

	void bindTexture(GLuint target = GL_TEXTURE1);
	void render(const Camera& camera, const Light& light,			   std::function<void(Shader&)> renderFunc);
	void render(const Camera& camera, const std::vector<Light>& light, std::function<void(Shader&)> renderFunc);

	int width() const;
	int height() const;

//private:
	Framebuffer fbo;
	Shader shader;
};
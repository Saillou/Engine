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

	void bindTextures(GLuint target = GL_TEXTURE0) const;
	void render(const Camera& camera, const std::vector<Light>& light, std::function<void(Shader&)> renderFunc);

	int width() const;
	int height() const;

private:
	std::vector<std::unique_ptr<Framebuffer>> fbos;
	Shader shader;
};
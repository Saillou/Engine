#pragma once

#include <vector>
#include <functional>

#include "../../Utils/Shader.hpp"
#include "../../Utils/Framebuffer.hpp"
#include "../../Light.hpp"
#include "../../Camera.hpp"

struct ShadowRender 
{
	ShadowRender();
	virtual ~ShadowRender() = default;

	ShadowRender& operator=(const ShadowRender&) = delete;
	ShadowRender(const ShadowRender&) = delete;
	ShadowRender(ShadowRender&&) = delete;

	void bindTextures(GLuint target = GL_TEXTURE0) const;
	void render(const Camera& camera, const std::vector<Light>& light, std::function<void(Shader&)> renderFunc);

	int width() const;
	int height() const;

private:
	std::vector<std::unique_ptr<Framebuffer>> fbos;
	Shader shader;
};
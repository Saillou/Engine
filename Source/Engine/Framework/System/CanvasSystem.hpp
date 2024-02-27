#pragma once

#include <vector>
#include <memory>
#include <unordered_set>

#include "../Core/ECS.hpp"
#include "../Core/System.hpp"
#include "../Component/CanvasComponent.hpp"

#include "../../Graphic/TextEngine.hpp"
#include "../../Graphic/Camera.hpp"
#include "../../Graphic/Base/Model/Primitive/Quad.hpp"

struct CanvasSystem :
	public System
{
	CanvasSystem(const Camera&);
	~CanvasSystem() = default;

	void init() override;
	void draw();

private:
	const Camera& m_camera;
};
#pragma once

#include <memory>
#include "PrimitiveHelper.hpp"

struct Cube : 
	private PrimitiveHelper
{
	static std::unique_ptr<Mesh> CreateMesh(bool sendToGpu = true);
};


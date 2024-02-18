#pragma once

#include <memory>
#include "PrimitiveHelper.hpp"

struct Cube : 
	private PrimitiveHelper
{
	static std::unique_ptr<Mesh> CreateMesh();
	static std::shared_ptr<Mesh> GetOne();
};


#pragma once

#include "../../Graphic/Base/Model/Model.hpp"
#include "Transform.hpp"
#include "Material.hpp"

struct BodyComponent 
{
	Model::Ref model = nullptr;

	Transform transform;
	Material  material;
};
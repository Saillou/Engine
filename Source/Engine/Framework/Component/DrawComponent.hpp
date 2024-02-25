#pragma once

#include "../../Graphic/Base/Model/Model.hpp"


struct DrawComponent 
{
	enum DrawType 
	{
		Hidden   = 0,
		Solid	 = (1 << 0),
		Geometry = (1 << 1),
		Custom   = (1 << 2), // To be kept last to retrieve the custom id
	};

	DrawType type = Hidden;
};
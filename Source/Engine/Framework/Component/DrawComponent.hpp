#pragma once

#include "../../Graphic/Base/Model/Model.hpp"


struct DrawComponent 
{
	// Render info
	enum DrawType 
	{
		Hidden = 0,
		Particle,
		Basic,
		Lights,
		Shadows,
		Geometry,
		Custom = 2048
	};

	DrawType type = Hidden;
};
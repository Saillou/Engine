#pragma once

namespace Render {
	enum DrawType {
		Particle,
		Basic,
		Lights,
		Shadows,
		Geometry,
		Custom = 4096
	};
}
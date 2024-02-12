#pragma once

#include <Engine/Events/Events.hpp>

struct GameEvents : public Event
{
	enum _Type {
		MouseHit_ = 666,
	};

	// -- Ray casting --
	struct MouseHit : public _Base {
		MouseHit(float x = 0, float y = 0, float z = 0):
			_Base(MouseHit_), x(x), y(y), z(z)
		{ }

		float x, y, z;
	};
};
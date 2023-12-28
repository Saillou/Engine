#pragma once

#include "Timer.hpp"

#include <memory>
#include <glm/glm.hpp>
#include <iostream>

struct Animator {
	struct Tweet {
		enum class Type {
			Linear, Quadratic
		};

		Tweet(const float start_offset, const float duration_sec, const Type type = Type::Linear);

		template <typename T> inline
			T update(const T& start, const T& end);

		float duration() const;

	private:
		float _get_rel_time();

		Timer::Chronometre m_time;
		const float m_offset;
		const float m_duration;
		const Type m_type;
	};
};

template<typename T>
inline T Animator::Tweet::update(const T& start, const T& end) 
{
	if (start == end)
		return end;

	const float a = _get_rel_time();

	switch (m_type) {
		case Type::Linear:		return end * a     + start * (1.0f - a);
		case Type::Quadratic:	return end * (a*a) + start * (1.0f - a*a);
	}
	return end;
}

template <>
inline std::string Animator::Tweet::update(const std::string& start, const std::string& end)
{
	return end;
}

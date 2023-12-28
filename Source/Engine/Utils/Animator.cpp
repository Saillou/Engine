#include "Animator.hpp"

Animator::Tweet::Tweet(const float start_offset, const float duration_sec, const Type type):
	m_offset(start_offset),
	m_duration(duration_sec),
	m_type(type)
{
	m_time.tic();
}

float Animator::Tweet::duration() const {
	return m_duration;
}

float Animator::Tweet::_get_rel_time() {
	return glm::clamp(
		(m_time.elapsed<Timer::millisecond>() / 1000.0f - m_offset) / m_duration,
		0.0f, 1.0f
	);
}

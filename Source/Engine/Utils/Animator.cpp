#include "Animator.hpp"

Animator::Tweet::Tweet(const float start_offset, const float duration_sec, const Type type):
	m_offset(start_offset),
	m_duration(duration_sec),
	m_type(type)
{
	reset();
}

Animator::Tweet& Animator::Tweet::operator=(const Animator::Tweet& src)
{
	return Animator::Tweet(src.m_offset, src.m_duration, src.m_type);
}

void Animator::Tweet::reset() {
	m_time.tic();
}

bool Animator::Tweet::ended() const
{
	return _get_rel_time() >= 1.0f - std::numeric_limits<float>::epsilon();
}

float Animator::Tweet::duration() const {
	return m_duration;
}

float Animator::Tweet::_get_rel_time() const {
	return glm::clamp(
		(m_time.elapsed<Timer::millisecond>() / 1000.0f - m_offset) / m_duration,
		0.0f, 1.0f
	);
}

#include "Animator.hpp"

Animator::Tweet::Tweet(const float start_offset, const float duration_sec, const Type type):
	m_offset(start_offset),
	m_duration(duration_sec),
	m_type(type)
{
	reset();
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

void Animator::Tweets::add(const std::string& name, const Tweet& tweet) {
	_tweets.push_back({ tweet, name });
}
void Animator::Tweets::reset() {
	_current_tweet_id = 0;

	if (!_tweets.empty())
		_tweets.front().reset();
}

const std::string& Animator::Tweets::current() const {
	if (ended()) {
		static std::string empty_string = "";
		return empty_string;
	}

	return _tweets[_current_tweet_id].name;
}
float Animator::Tweets::duration() const {
	float d = 0.0f;
	for (const auto& tw : _tweets)
		d += tw.duration();

	return d;
}
bool Animator::Tweets::ended() const {
	return _current_tweet_id == _tweets.size();
}
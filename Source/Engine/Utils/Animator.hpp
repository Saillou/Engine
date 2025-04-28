#pragma once

#include "Timer.hpp"

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <iostream>

struct Animator {
	struct Tweet {
		enum class Type {
			Linear, Quadratic
		};

		Tweet(const float start_offset = 0.0f, const float duration_sec = 1.0f, const Type type = Type::Linear);
		virtual ~Tweet() = default;

		void reset();
		bool ended() const;

		template <typename T> inline
			T update(const T& start, const T& end) const;

		float duration() const;

	private:
		float _get_rel_time() const;

		Timer::Chronometre m_time;
		float m_offset;
		float m_duration;
		Type m_type;
	};

	// An helper to serialize tweet
	struct Tweets 
	{
		void add(const std::string& name, const Tweet& tweet);
		void reset();
		bool ended() const;

		const std::string& current() const;
		template <typename T> inline
			T update(const T& start, const T& end);

		float duration() const;

	private:
		struct NamedTweet : public Tweet {
			NamedTweet(const Tweet& _tweet, std::string _name) :
				Tweet(_tweet), name(_name)
			{ };

			std::string name;
		};

		std::vector<NamedTweet> _tweets = {};
		size_t _current_tweet_id = 0;
	};
};

template<typename T>
inline T Animator::Tweet::update(const T& start, const T& end) const
{
	if (start == end)
		return end;

	const float a = _get_rel_time();

	switch (m_type) {
		case Type::Linear:		return T(end * a     + start * (1.0f - a));
		case Type::Quadratic:	return T(end * (a*a) + start * (1.0f - a*a));
	}
	return end;
}

template<typename T>
inline T Animator::Tweets::update(const T& start, const T& end)
{
	if (ended() || _tweets.empty())
		return end;

	if (_tweets[_current_tweet_id].ended()) {
		_current_tweet_id++;
		if (ended())
			return end;

		_tweets[_current_tweet_id].reset();
	}
	
	return _tweets[_current_tweet_id].update(start, end);
}
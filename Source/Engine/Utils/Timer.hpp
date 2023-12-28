#pragma once

#include <chrono>

// Overkill class for time measurement (tried stuff with template)
class Timer {
	using _time_point = std::chrono::time_point<std::chrono::system_clock>;

	// Base
	struct _time_policy {
		template <typename ReturnT> inline
			static ReturnT Get(const _time_point&) = 0;

		template <typename ReturnT> inline
			static ReturnT GetDiff(const _time_point& tp, const _time_point& ref) = 0;
	};

public:
	// Policies
	struct millisecond : public _time_policy {
		template <typename ReturnT> inline
			static ReturnT Get(const _time_point& tp)
		{
			return static_cast<ReturnT>(
				std::chrono::time_point_cast<std::chrono::milliseconds>(tp).time_since_epoch().count()
			);
		}

		template <typename ReturnT> inline
			static ReturnT GetDiff(const _time_point& tp, const _time_point& tp_ref)
		{
			return static_cast<ReturnT>(
				std::chrono::duration_cast<std::chrono::milliseconds>(tp - tp_ref).count()
			);
		}
	};

	struct microsecond : public _time_policy {
		template <typename ReturnT> inline
			static ReturnT Get(const _time_point& tp)
		{
			return static_cast<ReturnT>(
				std::chrono::time_point_cast<std::chrono::microseconds>(tp).time_since_epoch().count()
			);
		}

		template <typename ReturnT> inline
			static ReturnT GetDiff(const _time_point& tp, const _time_point& tp_ref)
		{
			return static_cast<ReturnT>(
				std::chrono::duration_cast<std::chrono::microseconds>(tp - tp_ref).count()
			);
		}
	};

	// Get time point
	static _time_point Now() {
		return std::chrono::system_clock::now();
	}

	// Get timestamp
	template <typename TimePolicy> inline
		static int64_t GetCurrentTime()
	{
		static_assert(std::is_base_of<_time_policy, TimePolicy>(), "Bad time policy");

		return TimePolicy::Get<int64_t>(Timer::Now());
	}

	// Instance
	struct Chronometre {
		Chronometre() : m_last_tic(Timer::Now()) {
			// ..
		}

		void tic() {
			m_last_tic = Timer::Now();
		}

		// Get elapsed time since last tic
		template <typename TimePolicy> inline
			int64_t elapsed() {
			static_assert(std::is_base_of<_time_policy, TimePolicy>(), "Bad time policy");

			return TimePolicy::GetDiff<int64_t>(Timer::Now(), m_last_tic);
		}

	private:
		_time_point m_last_tic;
	};
};
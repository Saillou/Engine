#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include "Common.hpp"

class Event {
protected:
	typedef int _Type;

	// Abstract base event
	struct _Base {
		_Base(_Type type);
		virtual ~_Base() = default;

		virtual const _Type type() const;

	private:
		_Type m_type;
	};

public:
	// [Static Emitter]
	template <typename T> inline
		static void Emit(const T& event, const void* _emitter = nullptr);

	// [Receiver]
	class Subscriber {
		friend Event;

	public:
		Subscriber();
		virtual ~Subscriber();

	protected:
		// Sub to any emitter
		template <class _subscriber, typename _message> inline
			void _subscribe(void(_subscriber::*callback)(const _message&));

		// Sub to a specific emitter
		template <class _emitter, class _subscriber, typename _message> inline
			void _subscribe(const _emitter*, void(_subscriber::* callback)(const _message&));

		void _unsubscribeAll();

	private:
		struct _callback {
			typedef std::function<void(const Event::_Base*)> crushy;
			crushy	func;
			void*	emitter;
		};

		std::unordered_map<_Type, std::vector<_callback>> _callbacks;
	};

private:
	static std::unordered_set<Subscriber*> _allSubscribers;
};

// -- Emitter --
template<typename T>
inline void Event::Emit(const T& event, const void* _emitter)
{
	static_assert(std::is_base_of<Event::_Base, T>(), "Can't emit non inherited BaseEvent.");

	for (Subscriber* subscriber : _allSubscribers) {
		for (const auto& callback : subscriber->_callbacks[event.type()]) {
			if (_emitter != callback.emitter)
				continue;

			callback.func(static_cast<const Event::_Base*>(&event));
		}
	}
}

// -- Receiver --
template<class _subscriber, typename _message>
void Event::Subscriber::_subscribe(void(_subscriber::*callback)(const _message&))
{
	_subscribe((void*)nullptr, callback);
}

template<class _emitter, class _subscriber, typename _message>
inline void Event::Subscriber::_subscribe(const _emitter* emitter, void(_subscriber::*callback)(const _message&))
{
	static_assert(std::is_base_of<Event::_Base, _message>(), "Can't subscribe to non inherited BaseEvent.");

	// Memorize type
	static const _Type type = _message{}.type();

	// Encapsulate the final callback
	_callback cbk;
	cbk.func = [=](const Event::_Base* msg) -> void
		{
			std::invoke(callback, static_cast<_subscriber*>(this), *static_cast<const _message*>(msg));
		};
	cbk.emitter = (void*)emitter;

	// Add to callbacks
	_callbacks[type].push_back(cbk);
}

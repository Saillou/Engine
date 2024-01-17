#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <functional>

#include "Common.hpp"

/*
	How to use:
	 - Create an XXEvent struct inherited from Event::_Base (or use one defined in CommonEvents.hpp)
	 - For the sender:
		`Event::Emit(xx_event)` | xx_event: an instance of XXEvent
	 - For the receiver, inherit the class `Event::Subscriber`, then, sub:
		`_subscribe(method);` | method signature: void(const XXEvent& evt);
*/

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
	// [Emitter]
	template <typename T> inline
		static void Emit(const T& event);

	// [Receiver]
	class Subscriber {
		friend Event;

	public:
		Subscriber();
		virtual ~Subscriber();

	protected:
		template <class _subscriber, typename _message> inline
			void _subscribe(void(_subscriber::*callback)(const _message&));

		void _unsubscribeAll();

	private:
		// Note: 
		//	these are temporary callbacks used to crush the _message type to _Base (inherited) type,
		//  the final callback provided by the subscriber is encapsulated in the crushy callback.
		typedef std::function<void(const Event::_Base*)> _crushyCallback;
		std::unordered_map<_Type, std::vector<_crushyCallback>> _callbacks;
	};

private:
	static std::unordered_set<Subscriber*> _allSubscribers;
};

// Template Implementation
template<typename T>
inline void Event::Emit(const T& event)
{
	static_assert(std::is_base_of<Event::_Base, T>(), "Can't emit non inherited BaseEvent.");

	for (Subscriber* subscriber : _allSubscribers) {
		for (const Subscriber::_crushyCallback& callback : subscriber->_callbacks[event.type()]) {
			callback(static_cast<const Event::_Base*>(&event));
		}
	}
}

template<class _subscriber, typename _message>
inline void Event::Subscriber::_subscribe(void(_subscriber::*finalCallback)(const _message&))
{
	static_assert(std::is_base_of<Event::_Base, _message>(), "Can't subscribe to non inherited BaseEvent.");

	// Memorize type
	static const _Type type = _message{}.type();

	// Encapsulate the final callback
	const _crushyCallback crushy = [=](const Event::_Base* msg) -> void 
	{
		std::invoke(finalCallback, static_cast<_subscriber*>(this), *static_cast<const _message*>(msg));
	};

	// Add to callbacks
	_callbacks[type].push_back(crushy);
}

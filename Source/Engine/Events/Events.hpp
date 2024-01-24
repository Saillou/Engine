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

	// [Emitter]
	class Emitter {
		friend Event;

	public:
		Emitter() = default;
		virtual ~Emitter() = default;

	protected:
		template <typename T> inline
			void _emit(const T& event);
	};

	// [Receiver]
	class Subscriber {
		friend Event;

	public:
		Subscriber();
		virtual ~Subscriber();

	protected:
		template <class _subscriber, typename _message> inline
			void _subscribe(void(_subscriber::*callback)(const _message&));

		template <class _emitter, class _subscriber, typename _message> inline
			void _subscribe(const _emitter*, void(_subscriber::* callback)(const _emitter*, const _message&));

		void _unsubscribeAll();

	private:
		// Note: 
		//	these are temporary callbacks used to crush the _message type to _Base (inherited) type,
		//  the final callback provided by the subscriber is encapsulated in the crushy callback.
		typedef std::function<void(const Event::_Base*)> _crushyCallback;
		typedef std::function<void(const void*, const Event::_Base*)> _crushySpeCallback;

		std::unordered_map<_Type, std::vector<_crushyCallback>> _callbacks;
		std::unordered_map<_Type, std::vector<_crushySpeCallback>> _spe_callbacks;
	};

private:
	static std::unordered_set<Subscriber*> _allSubscribers;
};

// -- Emitter --
template<typename T>
inline void Event::Emit(const T& event, const void* _emitter)
{
	static_assert(std::is_base_of<Event::_Base, T>(), "Can't emit non inherited BaseEvent.");

	// Broadcast event
	for (Subscriber* subscriber : _allSubscribers) {
		for (const Subscriber::_crushyCallback& callback : subscriber->_callbacks[event.type()]) {
			callback(static_cast<const Event::_Base*>(&event));
		}
	}

	if (!_emitter)
		return;

	// Select specifics
	for (Subscriber* subscriber : _allSubscribers) {
		for (const Subscriber::_crushySpeCallback& speCallback : subscriber->_spe_callbacks[event.type()]) {
			speCallback(static_cast<const void*>(_emitter), static_cast<const Event::_Base*>(&event));
		}
	}
}

template<typename T>
inline void Event::Emitter::_emit(const T& event) {
	Event::Emit(event, this);
}



// -- Receiver --
template<class _subscriber, typename _message>
inline void Event::Subscriber::_subscribe(void(_subscriber::*callback)(const _message&))
{
	static_assert(std::is_base_of<Event::_Base, _message>(), "Can't subscribe to non inherited BaseEvent.");

	// Memorize type
	static const _Type type = _message{}.type();

	// Encapsulate the final callback
	const _crushyCallback crushy = [=](const Event::_Base* msg) -> void 
	{
		std::invoke(callback, static_cast<_subscriber*>(this), *static_cast<const _message*>(msg));
	};

	// Add to callbacks
	_callbacks[type].push_back(crushy);
}

template<class _emitter, class _subscriber, typename _message>
inline void Event::Subscriber::_subscribe(const _emitter* emitter, void(_subscriber::*callback)(const _emitter*, const _message&))
{
	static_assert(std::is_base_of<Event::_Base, _message>(), "Can't subscribe to non inherited BaseEvent.");

	// Memorize type
	static const _Type type = _message{}.type();

	// Encapsulate the final callback
	const _crushySpeCallback crushy = [=](const void* emitter, const Event::_Base* msg) -> void
	{
		std::invoke(callback, 
			static_cast<_subscriber*>(this), 
			static_cast<const _emitter*>(emitter), 
			*static_cast<const _message*>(msg)
		);
	};

	// Add to callbacks
	_spe_callbacks[type].push_back(crushy);
}

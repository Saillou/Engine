#include "Events.hpp"

// Static init
std::unordered_set<Event::Subscriber*> Event::_allSubscribers;


// Private
Event::Subscriber::Subscriber() {
	_allSubscribers.insert(this);
}

Event::Subscriber::~Subscriber() {
	_allSubscribers.erase(this);
}

Event::_Base::_Base(_Type type) : m_type(type)
{
	// ..
}

// Identifier
const Event::_Type Event::_Base::type() const {
	return m_type;
}
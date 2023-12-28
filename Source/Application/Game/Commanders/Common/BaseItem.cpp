#include "BaseItem.hpp"

using namespace reactphysics3d;


// ------------------------ Base ------------------------
BaseItem::BaseItem():
    _body(nullptr),
    _pbody(nullptr)
{ }

// Getters
BaseItem::sBody BaseItem::body() const {
    return _body;
}

Physx::PBody BaseItem::pbody() const {
	return _pbody;
}

// Methods
void BaseItem::addAs(Physx::BodyType type) {
    _pbody = Physx::Add(_body ? _body : createBody(), type);
    _onAdd();
}

void BaseItem::setType(Physx::BodyType type) {
	if (!_pbody)
		return;

    _pbody->setType(([=]() -> ::BodyType {
		switch (type) {
			case Physx::BodyType::Dynamic:   return ::BodyType::DYNAMIC;
			case Physx::BodyType::Static:    return ::BodyType::STATIC;
			case Physx::BodyType::Kinematic: return ::BodyType::KINEMATIC;
		}
		return ::BodyType::STATIC;
	})());
}

void BaseItem::removePhysx() {
	Physx::Remove(_pbody);
	_pbody = nullptr;
}

void BaseItem::_onAdd() {
    // to be overrided
}

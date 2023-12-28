#include "Physx.hpp"
#include "../Events/CommonEvents.hpp"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>

#include <optional>
#include <vector>
#include <iostream>

using namespace glm;
using namespace reactphysics3d;

// Private impl
struct Physx::_impl
{
	struct BodyElement {
		BodyElement(std::shared_ptr<BaseBody> shape, RigidBody* rigid, Collider* collider):
			shape(shape),
			rigid(rigid),
			collider(collider)
		{
			// ..
		}

		std::shared_ptr<BaseBody> shape;
		RigidBody* rigid;
		Collider* collider;
	};

	Physx::_impl() :
		world(nullptr)
	{
		// Setup
		PhysicsWorld::WorldSettings settings;
		settings.gravity = Vector3(0.0f, 0.0f, -9.81f);

		// Create
		world = factory.createPhysicsWorld(settings);
	}

	// Hidden members
	PhysicsCommon factory;
	PhysicsWorld* world;
	std::unordered_set<std::shared_ptr<BodyElement>> bodies;
};

// Private
Physx& Physx::_get() {
	static Physx physx;
	return physx;
}

Physx::Physx() : p_impl(std::make_unique<_impl>()) 
{
	// ...
}
Physx::~Physx() 
{
	// Need a body due to unique_ptr pimpl
}

// Public
RigidBody* Physx::Add(std::shared_ptr<BaseBody> body, BodyType type) {
	auto& px = _get().p_impl;
	auto& world = px->world;

	// Convert pose(position, orientation) to world referential
	Transform pose(
		Vector3(
			body->position.x, 
			body->position.y, 
			body->position.z
		), 
		Quaternion::fromEulerAngles(Vector3(
			body->orientation.x,
			body->orientation.y,
			body->orientation.z
		))
	);

	// Create physical object
	RigidBody* phxBody = world->createRigidBody(pose);
	phxBody->setType(([=]() -> ::BodyType {
		switch (type) {
			case BodyType::Dynamic:   return ::BodyType::DYNAMIC;
			case BodyType::Static:    return ::BodyType::STATIC;
			case BodyType::Kinematic: return ::BodyType::KINEMATIC;
		}
		return ::BodyType::STATIC;
	})());

	// Manage collision
	Collider* collider = phxBody->addCollider(([&]() -> CollisionShape* {
		switch (body->type) {
		case BaseBody::ContactType::Sphere: 
			return px->factory.createSphereShape(
				body->dimensions.r
			);

		case BaseBody::ContactType::Box:
			return px->factory.createBoxShape(Vector3(
				body->dimensions.x,
				body->dimensions.y,
				body->dimensions.z
			));
		}
		return nullptr;
	})(), Transform::identity());

	// Add to world and memory
	px->bodies.insert(
		std::make_shared<_impl::BodyElement>(body, phxBody, collider)
	);

	return phxBody;
}

void Physx::Remove(PBody pbody) {
	auto& px = _get().p_impl;

	for (auto& element : px->bodies) {
		if (pbody == element->rigid) {
			px->bodies.erase(element);
			px->world->destroyRigidBody(pbody);
			break;
		}
	}
}

void Physx::Clear() {
	auto& px = _get().p_impl;
	auto& world = px->world;

	// Remove from world
	for (auto& elt : px->bodies) {
		world->destroyRigidBody(elt->rigid);
	}

	// Remove from memory
	px->bodies.clear();
}

void Physx::Compute(float delta_time_ms) {
	Physx& engine(_get());
	_impl& engine_impl(*engine.p_impl);

	// Magie here
	engine_impl.world->update(delta_time_ms / 1000.0f);

	// Update drawn bodies
	for (auto& element : engine_impl.bodies) {
		const Transform& transform	  = element->rigid->getTransform();
		const Vector3& position		  = transform.getPosition();
		const Quaternion& orientation = transform.getOrientation();
		
		element->shape->position	= vec3(position.x, position.y, position.z);
		element->shape->orientation = -eulerAngles(glm::quat(orientation.x, orientation.y, orientation.z, orientation.w));
	}
}
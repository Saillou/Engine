#pragma once

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>
#include <Engine/Framework/Component/DrawComponent.hpp>

#include <Engine/Graphic/Base/Model/Model.hpp>

#include <glm/glm.hpp>
#include <string>

struct Food {
	Food() :
		_entity(ECS::createEntity())
	{
		BodyComponent body;
		body.model = Model::Load(Model::SimpleShape::Sphere);
		body.transform.local = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
		body.transform.world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 2.0f, 2.0f));
		body.material.color = glm::vec4(0.2f, 0.7f, 1.0f, 0.7f);
		ECS::addComponent(_entity, body);

		DrawComponent draw;
		draw.type = DrawComponent::Solid;
		ECS::addComponent(_entity, draw);

		CollideComponent collide;
		ECS::addComponent(_entity, collide);
	}

	~Food() {
		ECS::destroyEntity(_entity);
	}

	Entity id() const {
		return _entity;
	}

	void fall(float dt_s) {
		constexpr float Gravity = 2.0f;

		BodyComponent& body = ECS::getComponent<BodyComponent>(_entity);
		body.transform.world = glm::translate(body.transform.world, glm::vec3(0, 0, -dt_s * Gravity));
	}

	bool is_colliding() {
		const CollideComponent& collide = ECS::getComponent<CollideComponent>(_entity);
		return collide.is_colliding;
	}

private:
	Entity _entity;
};
#pragma once

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>
#include <Engine/Framework/Component/DrawComponent.hpp>
#include <Engine/Framework/Component/CollideComponent.hpp>

#include <Engine/Graphic/Base/Model/Model.hpp>

#include <glm/glm.hpp>
#include <string>

struct Monster {
	Monster():
		_entity(ECS::createEntity()),
		_experience(0),
		_level(1)
	{
		BodyComponent body;
		body.model = Model::Load("Resources/objects/character/character.glb");
		body.transform.local = glm::rotate(glm::mat4(1.0f), glm::half_pi<float>(), glm::vec3(1, 0, 0));
		body.transform.local = glm::translate(body.transform.local, glm::vec3(0, 0.2f, 0));
		body.transform.world = glm::translate(glm::mat4(1.0f), glm::vec3(0, 2, 0));
		ECS::addComponent(_entity, body);

		DrawComponent draw;
		draw.type = DrawComponent::Solid;
		ECS::addComponent(_entity, draw);

		CollideComponent collide;
		ECS::addComponent(_entity, collide);
	}

	~Monster() {
		ECS::destroyEntity(_entity);
	}

	void earn_xp(int amount) {
		_experience += amount;

		_maybe_level_up(); // lol
	}

private:
	void _maybe_level_up() {
		if (_experience < 10)
			return;

		_experience = 0;
		_level++;

		BodyComponent& body = ECS::getComponent<BodyComponent>(_entity);
		body.transform.local = glm::scale(body.transform.local, glm::vec3(1.1f));
	}

	// Members
	Entity _entity;
	int _experience;
	int _level;
};
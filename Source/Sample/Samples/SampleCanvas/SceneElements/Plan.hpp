#pragma once

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>
#include <Engine/Framework/Component/DrawComponent.hpp>
#include <Engine/Framework/Component/CastComponent.hpp>

#include <Engine/Graphic/Base/Model/Model.hpp>

#include <glm/glm.hpp>
#include <string>

#include "Grid.hpp"

struct Plan {
	Plan() :
		_entity(ECS::createEntity())
	{
		BodyComponent body;
		body.model = Model::Load(Model::SimpleShape::Quad);
		body.transform.local = glm::scale(glm::mat4(1.0f), glm::vec3(5, 5, 1));
		body.transform.local = glm::translate(body.transform.local, glm::vec3(1, 1, 0));
		body.material.color = glm::vec4(0.2f, 0.7f, 1.0f, 0.7f);
		ECS::addComponent(_entity, body);

		DrawComponent draw;
		draw.type = DrawComponent::Solid;
		ECS::addComponent(_entity, draw);

		CastComponent cast;
		ECS::addComponent(_entity, cast);
	}

	~Plan() {
		ECS::destroyEntity(_entity);
	}

	Entity id() const { return _entity; }

	void selectCell(const glm::vec3& pos)
	{
		_grid.selectCell(pos);
	}

private:
	Entity _entity;
	Grid _grid;
};
#pragma once

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>
#include <Engine/Framework/Component/ParticleComponent.hpp>

#include <Engine/Graphic/Base/Model/Model.hpp>

#include <glm/glm.hpp>
#include <string>

struct Grid {
	Grid(): 
		_entity(ECS::createEntity()),
		_size(20)
	{
		BodyComponent body;
		body.model = Model::Load(Model::SimpleShape::Quad);
		body.transform.world = glm::translate(glm::mat4(1.0f), -glm::vec3(_size/2.0f, 2.0f, 0));
		ECS::addComponent(_entity, body);

		ParticleComponent cells;
		cells.type = ParticleComponent::Type::Geometry;
		cells.elements.reserve(_size * _size);

		for (size_t ix = 0; ix < _size; ix++) {
			for (size_t iy = 0; iy < _size; iy++) {
				ParticleComponent::Element cell;

				cell.color = glm::vec4(1.0f);
				cell.world = glm::translate(glm::mat4(1.0f), glm::vec3(ix, iy, 0));
				cells.elements.push_back(cell);
			}
		}
		ECS::addComponent(_entity, cells);
	}
	~Grid() 
	{
		ECS::destroyEntity(_entity);
	}

private:
	Entity _entity;
	size_t _size;
};
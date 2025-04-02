#pragma once

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/CanvasComponent.hpp>

struct CanvasEntity {
	CanvasEntity() :
		_entity(ECS::createEntity())
	{
		ECS::addComponent(_entity, CanvasComponent());
	}

	virtual ~CanvasEntity() {
		ECS::destroyEntity(_entity);
	}

	Entity id() const {
		return _entity;
	}

	CanvasComponent& get() {
		return ECS::getComponent<CanvasComponent>(_entity);
	}

private:
	Entity _entity;
};
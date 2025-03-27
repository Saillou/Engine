#pragma once

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/CanvasComponent.hpp>

struct CanvasElement {
	CanvasElement() :
		_entity(ECS::createEntity())
	{
		CanvasComponent canvas;
		ECS::addComponent(_entity, canvas);
	}

	virtual ~CanvasElement() {
		ECS::destroyEntity(_entity);
	}

	Entity id() const {
		return _entity;
	}

	CanvasComponent& canvas() {
		return ECS::getComponent<CanvasComponent>(_entity);
	}

private:
	Entity _entity;
};
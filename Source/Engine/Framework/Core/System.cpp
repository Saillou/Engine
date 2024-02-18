#include "System.hpp"


void System::addEntity(Entity entity)
{
	m_entities.insert(entity);
}

void System::removeEntity(Entity entity)
{
	m_entities.erase(entity);
}

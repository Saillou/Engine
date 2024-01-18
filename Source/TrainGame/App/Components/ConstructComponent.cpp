#include "ConstructComponent.h"

#include "glm/gtx/vector_angle.hpp"

#include "TrainGame/TrainGame/Grid.hpp"

namespace Thomas
{

    ConstructComponent::ConstructComponent(Transform* transform)
        : m_transform(transform)
    {
        _subscribe(&ConstructComponent::onMouseHit);
        _subscribe(&ConstructComponent::onKeyPressed);

        m_id = ComponentId::ConstructComponent;
        m_alive = true;
    }

    ConstructComponent::~ConstructComponent()
    {
    }

    void ConstructComponent::addGridComponent(GridComponent* grid)
    {
        m_grid = grid;
    }

    void ConstructComponent::update(float dt)
    {
    }

    void ConstructComponent::onMouseHit(const CommonEvents::MouseHit& evt)
    {
        if (!m_alive)
            return;

        const glm::vec3 newpos = Grid::alignPosition({ evt.x, evt.y, evt.z });
        m_transform->position = { newpos.x, newpos.y, evt.z };
    }

    void ConstructComponent::onKeyPressed(const CommonEvents::KeyPressed& evt)
    {
        if (!m_alive)
            return;

        switch (evt.key)
        {
        case 'G': construct(); break;
        }
    }

    void ConstructComponent::construct()
    {
        m_alive = false;

        if (!m_grid)
            return;

        m_grid->setState(GridComponent::GridComponentState::Visible);

    }
} // namespace Thomas
#include "ConstructComponent.h"

#include "glm/gtx/vector_angle.hpp"

#include "TrainGame/TrainGame/Grid.hpp"

namespace Thomas
{

    ConstructComponent::ConstructComponent(Transform* transform)
        : m_transform(transform)
    {
        _subscribe(&ConstructComponent::onMouseHit);
        _subscribe(&ConstructComponent::onMouseClick);

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

    void ConstructComponent::addObjectDeletionPtr(bool* ptr)
    {
        m_objectDeletion = ptr;
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

    void ConstructComponent::onMouseClick(const CommonEvents::MouseClicked& evt)
    {
        if (!m_alive)
            return;

        switch (evt.button)
        {
        case 0:
            construct();
            break;
        case 1:
            cancel();
            break;
        }
    
    }

    void ConstructComponent::construct()
    {
        m_alive = false;

        if (!m_grid)
            return;

        m_grid->setState(GridComponent::GridComponentState::Visible);   
    }

    void ConstructComponent::cancel()
    {
        if (!m_objectDeletion)
            return;

        *m_objectDeletion = true;
        m_alive = false;
    }
} // namespace Thomas
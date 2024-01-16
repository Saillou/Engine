#include "GridComponent.h"

#include "glm/gtx/vector_angle.hpp"
#include "TrainGame/TrainGame/Grid.hpp"
#include "TrainGame/TrainGame/GridCell.hpp"

namespace Thomas
{
    GridComponent::GridComponent(Transform* transform, const glm::vec2& size, const glm::vec2& offset)
        : m_transform(transform)
        , m_size(size)
        , m_offset(offset)
        , m_state(GridComponentState::Invisible)
    {
        m_id = ComponentId::GridComponent;
    }

    GridComponent::~GridComponent()
    {
    }

    void GridComponent::update(float dt)
    {
        switch (m_state)
        {
        case GridComponentState::Invisible:
            onInvinsible(dt);
            break;
        case GridComponentState::Visible:
            onVisible(dt);
            break;
        case GridComponentState::Construct:
            onConstruct(dt);
            break;
        }
    }

    void GridComponent::setState(GridComponentState state)
    {
        m_state = state;
    }

    void GridComponent::onInvinsible(float dt)
    {

    }

    void GridComponent::onVisible(float dt)
    {
        const std::pair<int64_t, int64_t> pos = Grid::getPosition(m_transform->position);

        for (int y = 0; y < m_size.y; y++)
        {
            for (int x = 0; x < m_size.x; x++)
            {
                Grid::setCellAtPosition(pos.first + x + m_offset.x, pos.second + y + m_offset.y, GridCell::CellType::Visible);
            }
        }
        
    }

    void GridComponent::onConstruct(float dt)
    {
    }

} // namespace Thomas
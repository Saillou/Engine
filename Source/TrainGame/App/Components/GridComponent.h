#pragma once

#include <list>

#include "Component.hpp"
#include "TrainGame/App/Objects/Transform.h"

namespace Thomas
{
    class GridComponent : public Component
    {
    public:
        enum class GridComponentState
        {
            Invisible = 0,
            Visible,
            Construct
        };

        GridComponent(Transform* transform, const glm::vec2& size, const glm::vec2& offset);
        ~GridComponent();

        void update(float dt) override;
        void setState(GridComponentState state);

    private:
        void onInvinsible(float dt);
        void onVisible(float dt);
        void onConstruct(float dt);

        // Misha: is there a better way of doing it ?
        Transform* m_transform = nullptr;
        
        GridComponentState m_state;
        glm::vec2 m_size;
        glm::vec2 m_offset;
    };

} // namespace Thomas
#include "GridSystem.h"

#include <Engine/Framework/Core/ECS.hpp>
#include <Engine/Framework/Component/BodyComponent.hpp>
#include <Engine/Framework/Component/ParticleComponent.hpp>

#include "TrainGame/Engine/Components/Transform.h"
#include "TrainGame/Engine/Components/Grid.h"
#include "TrainGame/Engine/Components/RenderComponent.h"
#include "TrainGame/TrainGame/TrainGameModels.hpp"

namespace Thomas
{
    void GridSystem::init()
    {
        Signature signature;
        signature.set(ECS::getComponentType<Transform>());
        signature.set(ECS::getComponentType<Grid>());

        ECS::setSystemSignature<GridSystem>(signature);
    }

    void GridSystem::start(const glm::vec3& pos, const glm::vec2& size)
    {
        m_position = pos;
        m_size = size;
    }

    void GridSystem::update(float dt)
    {
        for (const auto& cell : m_cells)
        {
            ECS::destroyEntity(cell.second.entity);
        }
        m_cells.clear();

        for (auto& entity : m_entities)
        {
            Transform& transform   = ECS::getComponent<Transform>(entity);
            Grid& grid             = ECS::getComponent<Grid>(entity);

            GridCell::CellType type = GridCell::CellType::None;
            const auto coordinates = getPosition(transform.position);

            switch (grid.m_state)
            {
            case Grid::GridComponentState::Construct:
                type = GridCell::CellType::ConstructOk;
                break;
            case Grid::GridComponentState::Visible:
                type = GridCell::CellType::Visible;
                break;
            }

            for (size_t i = 0; i < grid.m_cells.size(); ++i)
            {
                if (!grid.m_cells[i])
                    continue;

                const int y = static_cast<int>(i) / grid.m_size.x;
                const int x = static_cast<int>(i) - (y * grid.m_size.y);

                setCellAtPosition(coordinates.first + x + grid.m_offset.x, coordinates.second + y + grid.m_offset.y, type);
            }
        }

        for (auto& cell : m_cells)
        {
            Entity ent = ECS::createEntity();
            cell.second.entity = ent;

            RenderComponent renderComponent;
            renderComponent.mobility = RenderComponent::Mobility::Static;
            renderComponent.modelId = ModelId::CubeBasic;
            renderComponent.material.cast_shadow = false;

            switch (cell.second.type)
            {
            case GridCell::CellType::ConstructOk:
                renderComponent.material.diffuse_color = { 0.4f, 0.7f, 0.4f, 0.3f };
                break;
            case GridCell::CellType::ConstructBad:
                renderComponent.material.diffuse_color = { 0.7f, 0.4f, 0.4f, 0.3f };
                break;
            case GridCell::CellType::Visible:
                renderComponent.material.diffuse_color = { 0.7f, 0.6f, 0.4f, 0.3f };
                break;
            default:
                renderComponent.material.diffuse_color = { 0.f, 0.f, 0.f, 0.f };
                break;
            }
            ECS::addComponent(ent, renderComponent);

            Transform transform;
            transform.position = getPosition(cell.first.first, cell.first.second);
            transform.scale = 0.15f * glm::vec3{ 0.3f,0.3f, 0.01f };
            transform.rotation = { 0.f,0.f,0.f };
            ECS::addComponent(ent, transform);

            BodyComponent bodyComponent;
            bodyComponent.model = Model::Load(Model::SimpleShape::Cube);
            bodyComponent.transform.local = GameModelTable::getModelById(renderComponent.modelId).transform.model;
            bodyComponent.transform.world = glm::mat4(1.0f);
            ECS::addComponent(ent, bodyComponent);

            ParticleComponent particle;
            particle.type = ParticleComponent::Type::Solid;
            particle.elements = { ParticleComponent::Element{ renderComponent.material.diffuse_color, transform.getMat4()} };
            ECS::addComponent(ent, particle);
        }
    }

    glm::vec3 GridSystem::getPosition(int x, int y)
    {
        return {
            x * m_size.x + m_position.x,
            y * m_size.y + m_position.y,
            m_position.z
        };
    }

    std::pair<int, int> GridSystem::getPosition(const glm::vec3& pos)
    {
        const int x = static_cast<int>((pos.x + m_position.x) / m_size.x);
        const int y = static_cast<int>((pos.y + m_position.y) / m_size.y);
        return { x,y };
    }

    glm::vec3 GridSystem::alignPosition(const glm::vec3& pos)
    {
        auto pair = getPosition(pos);
        return getPosition(pair.first, pair.second);
    }

    void GridSystem::setCellAtPosition(int x, int y, GridCell::CellType type)
    {
        GridCell cell;
        cell.type = type;
        
        auto it = m_cells.find({ x,y });

        if (it != m_cells.end())
        {
            if (it->second.type == GridCell::CellType::ConstructBad)
                return;

            const bool cond1 = type == GridCell::CellType::ConstructOk && it->second.type == GridCell::CellType::Visible;
            const bool cond2 = type == GridCell::CellType::Visible && it->second.type == GridCell::CellType::ConstructOk;
            if (cond1 || cond2)
                cell.type = GridCell::CellType::ConstructBad;
        }

        m_cells[{x, y}] = cell;
    }
} // namespace Thomas
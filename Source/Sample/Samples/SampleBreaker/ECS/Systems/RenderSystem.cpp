#include "RenderSystem.h"

#include "../Components/Transform.h"
#include "../Components/RenderComponent.h"

void RenderSystem::init()
{
    Signature signature;
    signature.set(ECS::getComponentType<Breaker::Transform>());
    signature.set(ECS::getComponentType<Breaker::RenderComponent>());

    ECS::setSystemSignature<RenderSystem>(signature);

    loadModels();
}

void RenderSystem::loadModels()
{
    m_gameModels[Breaker::ModelId::CubeBasic].model = Model::Create(Model::Cube);
    m_gameModels[Breaker::ModelId::CubeBasic].drawType = Render::DrawType::Basic;
    m_gameModels[Breaker::ModelId::CubeBasic].model->localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));

    m_gameModels[Breaker::ModelId::SphereBasic].model = Model::Create(Model::Sphere);
    m_gameModels[Breaker::ModelId::SphereBasic].drawType = Render::DrawType::Basic;
    m_gameModels[Breaker::ModelId::SphereBasic].model->localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
}

void RenderSystem::update(Renderer& renderer)
{
    m_timer.tic();

    for (auto& entity : m_entities)
    {
        Breaker::Transform& transform = ECS::getComponent<Breaker::Transform>(entity);
        Breaker::RenderComponent& renderComponent = ECS::getComponent<Breaker::RenderComponent>(entity);

        if (renderComponent.mobility == Breaker::RenderComponent::Mobility::Static)
            continue;

        const GameModel& gameModel = m_gameModels[renderComponent.modelId];

        m_batchedPoses[renderComponent.modelId][m_batchIdMap[entity]] = { transform.getMat4() * gameModel.model->localTransform };
        m_batchedMaterials[renderComponent.modelId][m_batchIdMap[entity]] = renderComponent.material;
    }

    for (auto& model : m_gameModels)
    {
        model.second.model->transforms = m_batchedPoses[model.first];
        model.second.model->materials = m_batchedMaterials[model.first];

        renderer.draw(model.second.drawType, model.second.model);
    }

    {
        static float average = 0.f;
        static size_t amount = 0;

        ++amount;
        average += (m_timer.elapsed<Timer::microsecond>() / 1000.f);

        std::ostringstream ss;
        ss << "Render batch models in: " << (m_timer.elapsed<Timer::microsecond>() / 1000.f) << " ms";
        std::string s(ss.str());

        std::ostringstream ss2;
        ss2 << "Render batch average: " << (average / amount) << " ms";
        std::string s2(ss2.str());

        renderer.text(s, 30, 30, 1.f);
        renderer.text(s2, 30, 80, 1.f);
    }
}

void RenderSystem::addEntity(Entity entity)
{
    if (m_batchIdMap.find(entity) != m_batchIdMap.end())
        return;

    auto& renderComponent = ECS::getComponent<Breaker::RenderComponent>(entity);
    auto& transform = ECS::getComponent<Breaker::Transform>(entity);

    const GameModel& gameModel = m_gameModels[renderComponent.modelId];

    m_batchedPoses[renderComponent.modelId].push_back(transform.getMat4() * gameModel.model->localTransform);
    m_batchedMaterials[renderComponent.modelId].push_back(renderComponent.material);

    m_batchIdMap[entity] = m_batchedPoses[renderComponent.modelId].size() - 1;
    m_batchModelMap[entity] = renderComponent.modelId;
    m_positionMap[m_batchedPoses[renderComponent.modelId].size() - 1] = entity;
}

void RenderSystem::removeEntity(Entity entity)
{
    if (m_batchIdMap.find(entity) == m_batchIdMap.end())
        return;

    const Breaker::ModelId modelId = m_batchModelMap[entity];
    const size_t position = m_batchIdMap[entity];
    const bool isLast = position == m_batchedPoses[modelId].size() - 1;

    if (isLast)
    {
        m_batchedPoses[modelId].pop_back();
        m_batchedMaterials[modelId].pop_back();
        m_batchIdMap.erase(entity);
    }
    else
    {
        const size_t indexOfLast = m_batchedPoses[modelId].size() - 1;
        const Entity lastEntity = m_positionMap[indexOfLast];

        m_batchedPoses[modelId][position] = m_batchedPoses[modelId][indexOfLast];
        m_batchedMaterials[modelId][position] = m_batchedMaterials[modelId][indexOfLast];
        m_batchIdMap[lastEntity] = position;
        m_positionMap[position] = lastEntity;

        m_batchedPoses[modelId].pop_back();
        m_batchedMaterials[modelId].pop_back();
        m_batchIdMap.erase(entity);
    }
}

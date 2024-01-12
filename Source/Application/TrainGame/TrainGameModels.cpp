#include "TrainGameModels.hpp"

GameModelTable& GameModelTable::get()
{
    static GameModelTable table;
    return table;
}

GameModelTable::GameModelTable()
{
    GameModel model;
    model.modelId = ModelId::None;

    m_data[ModelId::None] = model;
}

const GameModel& GameModelTable::getModelById(ModelId id)
{
    GameModelTable& table = get();
    
    auto it = table.m_data.find(id);
    if (it == table.m_data.end())
    {
        return table.m_data[ModelId::None];
    }
    else
    {
        GameModel model = table.m_data[id];
        return table.m_data[id];
    }
    //return it == table.m_data.end() ? GameModel() : table.m_data[id];
}

bool GameModelTable::addGameModel(ModelId id, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotVec, float rotation)
{
    GameModelTable& table = get();

    auto it = table.m_data.find(id);
    if (it != table.m_data.end())
        return false;

    GameModel model;
    model.modelId = id;
    model.localRotation = rotation;
    model.localPosition = pos;
    model.localScale = scale;
    model.localRotationVector = rotVec;

    table.m_data[id] = model;
    return true;
}

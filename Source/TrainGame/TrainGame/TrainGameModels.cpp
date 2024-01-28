#include "TrainGameModels.hpp"

#include "TrainGame/App/Objects/GameModel.hpp"

namespace Thomas
{

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

    bool GameModelTable::addGameModel(ModelId id, const Transform& transform)
    {
        GameModelTable& table = get();

        auto it = table.m_data.find(id);
        if (it != table.m_data.end())
            return false;

        GameModel model;
        model.modelId = id;
        model.transform = transform;
        model.transform.calculateMat4();

        table.m_data[id] = model;
        return true;
    }
} // namespace Thomas
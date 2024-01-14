#pragma once

#include <memory>

#include "TrainGame/App/Objects/GameModel.hpp"

/*
*  Misha: I don't know if i want to keep it as a Singletone,
*  because not sure how I want to do it,
*  but for now I will keep it like this and I will see where it brings me...
*/

namespace Thomas
{

    class GameModelTable
    {
    public:
        static GameModelTable& get();

        static const GameModel& getModelById(ModelId id);
        static bool addGameModel(ModelId id, const glm::vec3& pos, const glm::vec3& scale, const glm::vec3& rotVec, float rotation);

    private:
        GameModelTable();
        GameModelTable(const GameModelTable& other) = delete;
        GameModelTable(const GameModelTable&& other) = delete;

        std::unordered_map<ModelId, GameModel> m_data;
    };
} // namespace Thomas
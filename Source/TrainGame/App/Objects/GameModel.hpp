#pragma once

#include <memory>

#include <Engine/Graphic/Base/Model/ObjectModel.hpp>

namespace Thomas
{

    enum class ModelId
    {
        None = 0,
        Locomotive,
        Wagon
    };

    struct Transform
    {
        glm::vec3   position;
        glm::vec3   scale;
        glm::vec3   rotationVector;
        float       rotation;
    };

    struct GameModel
    {
        GameModel()
            : modelId(ModelId::None)
            , transform({})
        {}

        GameModel(const GameModel& other)
        {
            modelId = other.modelId;
            transform = other.transform;
        }

        ModelId     modelId;
        Transform   transform; // local transform of the Model

        // todo: add quat
    };

} // namespace Thomas
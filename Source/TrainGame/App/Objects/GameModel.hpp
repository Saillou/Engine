#pragma once

#include <memory>

#include "TrainGame/App/Objects/Transform.h"
#include <Engine/Graphic/Base/Model/Entity.hpp>

namespace Thomas
{

    enum class ModelId
    {
        None = 0,
        Locomotive,
        Wagon,
        Track,
        TrackLeft,
        TrackRight,
        Building1,
        CubeBasic,
        CubeGeometry
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
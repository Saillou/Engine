#pragma once

#include <memory>

#include "TrainGame/Engine/Components/Transform.h"
#include "TrainGame/Engine/Components/Material.h"
#include <Engine/Graphic/Base/Model/Model.hpp>

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
        CubeGeometry,
        SphereBasic,
        SphereGeometry
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

        ModelId      modelId;
        Transform   transform; // local transform of the Model

        // todo: add quat
    };

} // namespace Thomas
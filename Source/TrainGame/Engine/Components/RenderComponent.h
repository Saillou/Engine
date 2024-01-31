#pragma once

#include "TrainGame/App/Objects/GameModel.hpp"

namespace Thomas
{
    struct RenderComponent
    {
        enum class Mobility
        {
            Static = 0,
            Dynamic
        };

        bool        shouldRender    = true;
        ModelId     modelId         = ModelId::None;
        Mobility    mobility        = Mobility::Static;
        Material    material        = { {1.f, 1.f, 1.f, 1.f}, true };
    };
} // namespace Thomas
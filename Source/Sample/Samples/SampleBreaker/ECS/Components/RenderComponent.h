#pragma once

#include <Engine/Graphic/Base/Model/Material.hpp>

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
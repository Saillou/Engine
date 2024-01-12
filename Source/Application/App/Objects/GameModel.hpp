#pragma once

#include <memory>

#include <Engine/Graphic/Base/Model/ObjectModel.hpp>

enum class ModelId
{
    None = 0,
    Locomotive,
    Wagon
};

struct GameModel
{
    GameModel()
        : modelId(ModelId::None)
        , localRotation(0.f)
        , localPosition({0})
        , localScale({1})
        , localRotationVector({1,0,0})
    {}

    GameModel(const GameModel& other)
    {
        modelId = other.modelId;
        localRotation = other.localRotation;
        localPosition = other.localPosition;
        localScale = other.localScale;
        localRotationVector = other.localRotationVector;
    }

    ModelId     modelId;
    float       localRotation;
    glm::vec3   localPosition;
    glm::vec3   localScale;
    glm::vec3   localRotationVector;
};
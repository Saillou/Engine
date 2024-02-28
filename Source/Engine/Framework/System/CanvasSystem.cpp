#include "CanvasSystem.hpp"

#include "../Component/CanvasComponent.hpp"
#include "../../Graphic/Base/Canvas/ShapeMesh.hpp"
#include "../../Graphic/ShaderManager.hpp"

CanvasSystem::CanvasSystem(const Camera& camera) :
	m_camera(camera)
{
}

void CanvasSystem::init()
{
    Signature signature;

    signature.set(ECS::getComponentType<CanvasComponent>());

    ECS::setSystemSignature<CanvasSystem>(signature);
}

void CanvasSystem::draw()
{
    for (Entity entity : m_entities) {
        _drawFromComponent(ECS::getComponent<CanvasComponent>(entity));
    }
}

// Private
void CanvasSystem::_drawFromComponent(const CanvasComponent& component)
{
    _setShapeShader(component.context);

    for (const CanvasShape& shape : component.shapes)
    {
        ShaderManager::Get(CookType::Shape)
            .set("Color", shape.color);

        ShapeMesh::Draw(shape);
    }
}

void CanvasSystem::_setShapeShader(const CanvasContext& context) 
{
    ShaderManager::Get(CookType::Shape)
        .use()
        .set("Ratio", context.dimensionsFormat == CanvasContext::Dimensions::Absolute ?
            glm::vec2(1.0f):
            glm::vec2(1.0f)
        )
        .set("Projection", glm::vec4(0.0f, m_camera.screenSize.x, 0.0f, m_camera.screenSize.y))
    ;
}


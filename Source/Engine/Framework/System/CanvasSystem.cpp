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
    // Can be optimized by not updating the vbo meshes for each different meshes
    for (Entity entity : m_entities) {
        _drawFromComponent(ECS::getComponent<CanvasComponent>(entity));
    }
}

// Private
void CanvasSystem::_drawFromComponent(const CanvasComponent& component)
{
    _setShapeShader(component.context);

    for (const CanvasShape& shape : component.shapes) {
        switch (shape.shapeType)
        {
        case CanvasShape::ShapeType::Shape: 
            ShaderManager::Get(CookType::Shape)
                .use()
                .set("Color", shape.color);

            switch (shape.drawType)
            {
            case CanvasShape::DrawType::Fill:   ShapeMesh::Fill(shape);   break;
            case CanvasShape::DrawType::Stroke: ShapeMesh::Stroke(shape); break;

            default: continue;
            }
            break;

        case CanvasShape::ShapeType::Text:
            float color_unit = component.context.colorFormat == CanvasContext::ColorFormat::Uint8 ? (1.0f / 255.0f) : 1.0f;
            auto color = color_unit * shape.color;

            switch (shape.drawType)
            {
            case CanvasShape::DrawType::Fill: 
                TextEngine::Write(shape.dataText.text, shape.dataText.start.x, shape.dataText.start.y, shape.dataText.fontSize, color);
            break;

            case CanvasShape::DrawType::Stroke:
            default: continue;
            }
            break;
        }
    }
}

void CanvasSystem::_setShapeShader(const CanvasContext& context) 
{
    ShaderManager::Get(CookType::Shape)
        .use()
        .set("ColorUnit", context.colorFormat == CanvasContext::ColorFormat::Uint8 ?
            (1.0f / 255.0f) :
            1.0f
        )
        .set("Projection", context.dimensionsFormat == CanvasContext::Dimensions::Absolute ?
            glm::ortho(0.0f, m_camera.screenSize.x, 0.0f, m_camera.screenSize.y) :
            glm::mat4(1.0f)
        )
        ;
}

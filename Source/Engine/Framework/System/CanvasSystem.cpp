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
    float r = component.context.colorFormat == CanvasContext::ColorFormat::Uint8 ? 1.0f / 255.0f : 1.0f;
    _setShapeShader(component.context);

    for (const CanvasShape& shape : component.shapes)
    {
        ShaderManager::Get(CookType::Shape)
            .set("Color", r * shape.color)
        ;

        if (shape.type == CanvasShape::DrawType::Fill) {
            ShapeMesh::Draw(shape);
        }

        // TODO: use a geometrie shader instead
        if (shape.type == CanvasShape::DrawType::Stroke) {
            glPolygonMode(GL_FRONT, GL_LINE);
            glPolygonMode(GL_BACK, GL_LINE);

            ShapeMesh::Draw(shape);

            glPolygonMode(GL_FRONT, GL_FILL);
            glPolygonMode(GL_BACK, GL_FILL);
        }
    }
}

void CanvasSystem::_setShapeShader(const CanvasContext& context) 
{
    ShaderManager::Get(CookType::Shape)
        .use()
        .set("Projection", context.dimensionsFormat == CanvasContext::Dimensions::Absolute ?
            glm::ortho(0.0f, m_camera.screenSize.x, 0.0f, m_camera.screenSize.y): 
            glm::mat4(1.0f)
        )
    ;
}


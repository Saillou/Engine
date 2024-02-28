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
    for (Entity entity : m_entities) 
    {
        const CanvasComponent& canvas = ECS::getComponent<CanvasComponent>(entity);
        
        _setShapeShader(canvas.context);

        for (const CanvasShape& shape : canvas.shapes) {
            ShapeMesh::Draw(shape);
        }
    }
}

void CanvasSystem::_setShapeShader(const CanvasContext& context) 
{
    ShaderManager::Get(CookType::Shape)
        .use()
        .set("LocalModel", context.dimensionsFormat == CanvasContext::Dimensions::Absolute ?
            glm::mat4(1.0f): 
            glm::mat4(1.0f)
        )
        .set("Projection", context.dimensionsFormat == CanvasContext::Dimensions::Absolute ?
            glm::ortho(0.0f, m_camera.screenSize.x, 0.0f, m_camera.screenSize.y):
            glm::mat4(1.0f)
        );
}

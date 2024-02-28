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
        const CanvasContext& context = canvas.context;

        for (const CanvasShape& shape : canvas.shapes) {
            ShapeMesh::Draw(shape);
        }
    }
}

void CanvasSystem::_setShapeShader() 
{

}

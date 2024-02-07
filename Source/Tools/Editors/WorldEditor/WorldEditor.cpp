#include "WorldEditor.h"

// -------- Utilities --------
static Entity __create_tile_with_texture(const std::string& texturePath) {
    Entity entity(Entity::SimpleShape::Quad);

    // Transform:
    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)
    entity.localPose() = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 1.0f)), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    // Define opacity < 1: trick renderer with reordering
    entity.localMaterial() = Material{ glm::vec4(0, 0, 0, 0.9f), false }; 

    // Set texture on the first (and only) mesh
    entity.model().root()->meshes.front()->textures().push_back(
        TextureData{ "texture_diffuse", std::make_unique<Texture>(texturePath) }
    );

    return entity;
}

static Entity __create_tile_with_rgba(const glm::vec4& colorRGBA) {
    Entity entity(Entity::SimpleShape::Quad);

    // channel color 8-bits to float
    entity.localMaterial() = Material{ colorRGBA / 255.0f, false };

    return entity;
}

// -------- Editor --------
WorldEditor::WorldEditor(Scene& scene) : 
    Editor(scene)
{ }

void WorldEditor::onEnter()
{
    // Change app draw style to be able to reorder entities and compute shadows
    m_scene.directDraw(false);

    // Set menu
    m_menu.reset();

    // Set Scene
    m_scene.lights() = {};

    m_scene.camera().position  = glm::vec3(0.0f, -5.0f, 0.15f);
    m_scene.camera().direction = glm::vec3(0, 0, 0);

    // Create tiles
    m_entities["grass"] = __create_tile_with_texture("Resources/textures/grass.png");
    m_entities["earth"] = __create_tile_with_rgba(glm::vec4(185, 122, 87, 255));

    // Create world
    m_entities["grass"].poses() = {
        glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)), glm::vec3(-0.25f, 0.1f, 0.0f)),
        glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)), glm::vec3(+0.25f, 0.2f, 0.0f)),
    };

    m_entities["earth"].poses() = {
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)),
    };
}

void WorldEditor::onExit()
{ 
    // Put back original app state
    m_scene.directDraw(true);
}

void WorldEditor::onUpdate()
{
    _drawScene();
    _drawMenu();
}

// - Private -
void WorldEditor::_drawMenu() {
    m_menu.show();
}

void WorldEditor::_drawScene() {
    Renderer& render = m_scene.renderer();

    static float a = 0.f;
    static float x = 0.03f;
    a += x;

    if (std::abs(a) > 3.14f)
        x *= -1;

    m_entities["quad"].setPosesWithMaterials(
        { glm::rotate(glm::mat4(1.0f), a, glm::vec3(1, 0, 0)) },
        { Material{glm::vec4(1, 0, 0, 1), false} }
    );

    for (auto& entity : m_entities) {
        render.draw(Render::DrawType::Lights, entity.second);
    }
}
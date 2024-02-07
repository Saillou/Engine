#include "WorldEditor.h"

// -------- Utilities --------
static const glm::mat4 s_identity = glm::mat4(1.0f);

static Entity __create_tile_with_texture(const std::string& texturePath) {
    Entity entity(Entity::SimpleShape::Quad);

    // Transform:
    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)
    //  - scale in half because quads are 2x2 by default
    entity.localPose() = glm::scale(glm::mat4(entity.localPose()), glm::vec3(0.5f));
    entity.localPose() = glm::translate(glm::mat4(entity.localPose()), glm::vec3(0, 0, 1.0f));
    entity.localPose() = glm::rotate(glm::mat4(entity.localPose()), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

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

    // Transform:
    //  - scale in half because quads are 2x2 by default
    entity.localPose() = glm::scale(s_identity, glm::vec3(0.5f));

    // channel color 8-bits to float
    entity.localMaterial() = Material{ colorRGBA / 255.0f, false };

    return entity;
}

static glm::mat4 __pose(const glm::vec2& position, float scale = 1.0f) {
    glm::mat4 model_translate = glm::translate(s_identity, glm::vec3(position, 0.0f));
    glm::mat4 model_scale     = glm::scale(model_translate, glm::vec3(scale));

    return model_scale;
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
        __pose(glm::vec2(-0.25f, 0.1f), 0.5f),
        __pose(glm::vec2(-0.25f, 0.1f), 1.5f)
    };

    m_entities["earth"].poses() = {
        __pose(glm::vec2(0, 0)),
        __pose(glm::vec2(1.01f, 0))
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

    for (auto& entity : m_entities) {
        render.draw(Render::DrawType::Lights, entity.second);
    }
}
#include "WorldEditorHelper.hpp"

// -------- Utilities --------
Entity WorldEditorHelper::train() {
    Entity entity("Resources/objects/train/locomotive.glb");

    // Transform:
    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)
    entity.localPose() = glm::scale(glm::mat4(entity.localPose()), glm::vec3(0.5f));
    entity.localPose() = glm::translate(glm::mat4(entity.localPose()), glm::vec3(0, 0, 0.17f));
    entity.localPose() = glm::rotate(glm::mat4(entity.localPose()), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    return entity;
}

Entity WorldEditorHelper::tile_with_texture(const std::string& texturePath) {
    Entity entity(Entity::SimpleShape::Quad);

    // Transform:
    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)
    //  - scale in half because quads are 2x2 by default
    entity.localPose() = glm::scale(glm::mat4(entity.localPose()), glm::vec3(0.5f));
    entity.localPose() = glm::translate(glm::mat4(entity.localPose()), glm::vec3(0, 0, 1.0f));
    entity.localPose() = glm::rotate(glm::mat4(entity.localPose()), glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    // Define opacity < 1: trick renderer with reordering
    entity.localMaterial() = Material{ glm::vec4(0, 0, 0, 0.9f) };

    // Set texture on the first (and only) mesh
    entity.model().root()->meshes.front()->textures().push_back(
        TextureData{ "texture_diffuse", std::make_unique<Texture>(texturePath) }
    );

    return entity;
}

Entity WorldEditorHelper::tile_with_rgba(const glm::vec4& colorRGBA) {
    Entity entity(Entity::SimpleShape::Quad);

    // Transform:
    //  - scale in half because quads are 2x2 by default
    entity.localPose() = glm::scale(glm::mat4(entity.localPose()), glm::vec3(0.5f));

    // channel color 8-bits to float
    entity.localMaterial() = Material{ colorRGBA / 255.0f, false };

    return entity;
}

glm::mat4 WorldEditorHelper::pose(const glm::vec2& position) {
    return glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
}

glm::mat4 WorldEditorHelper::pose_scale(const glm::vec2& position, float scale) {
    return glm::scale(pose(position), glm::vec3(scale));
}

glm::mat4 WorldEditorHelper::pose_rot(const glm::vec2& position, float angle) {
    return glm::rotate(pose(position), angle, glm::vec3(0, 0, 1));
}
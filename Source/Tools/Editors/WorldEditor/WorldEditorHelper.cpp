#include "WorldEditorHelper.hpp"

// -------- Utilities --------
Model::Ref WorldEditorHelper::train() {
    Model::Ref model = Model::Create("Resources/objects/train/locomotive.glb");

    // Transform:
    glm::mat4& pose(model->localPose);

    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)

    pose = glm::scale(pose, glm::vec3(0.5f));
    pose = glm::translate(pose, glm::vec3(0, 0, 0.17f));
    pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    return model;
}

Model::Ref WorldEditorHelper::tile_with_texture(const std::string& texturePath) {
    Model::Ref model = Model::Create(Model::Quad);

    // Transform:
    glm::mat4& pose(model->localPose);

    //  - translate on z for having the ground level at 0
    //  - rotate for having Y as depth (instead of Z because i prefer)
    //  - scale in half because quads are 2x2 by default

    pose = glm::scale(pose, glm::vec3(0.5f));
    pose = glm::translate(pose, glm::vec3(0, 0, 1.0f));
    pose = glm::rotate(pose, glm::pi<float>() / 2.0f, glm::vec3(1, 0, 0));

    // Define opacity < 1: trick renderer with reordering
    model->localMaterial = Material{ glm::vec4(0, 0, 0, 0.9f) };

    // Set texture on the first (and only) mesh
    model->root->meshes.front()->textures().push_back(
        TextureData{ "texture_diffuse", std::make_unique<Texture>(texturePath) }
    );

    return model;
}

Model::Ref WorldEditorHelper::tile_with_rgba(const glm::vec4& colorRGBA) {
    Model::Ref model = Model::Create(Model::Quad);

    // Transform:
    glm::mat4& pose(model->localPose);

    //  - scale in half because quads are 2x2 by default
    pose = glm::scale(pose, glm::vec3(0.5f));

    // channel color 8-bits to float
    model->localMaterial = Material{ colorRGBA / 255.0f, false };

    return model;
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
#include "ObjectModel.hpp"

ObjectModel::ObjectModel(const std::string& path) :
    _object(path)
{
    _shader.
        attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")

            .add_var("uniform", "mat4", "model")
            .add_var("uniform", "mat4", "view")
            .add_var("uniform", "mat4", "projection")

            .add_var("out", "vec2", "TexCoords")

            .add_func("void", "main", "", R"_main_(
                TexCoords = aTexCoords;    
                gl_Position = projection * view * model * vec4(aPos, 1.0);
            )_main_").str()
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "texture_diffuse1")
            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = texture(texture_diffuse1, TexCoords);
            )_main_").str()
        )
    .link();
}

void ObjectModel::draw(const Camera& camera, const glm::mat4& model) {
    _shader
        .use()
        .set("projection",  camera.projection)
        .set("view",        camera.modelview)
        .set("model",       model);

    _object.draw(_shader);
}


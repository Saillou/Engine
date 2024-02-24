#pragma once

#include "_ShaderHelper.hpp"

// -- Definition --
struct ShaderSkybox : public ShaderHelper {
	static inline std::shared_ptr<Shader> Create();
};

// -- Implementation --
inline std::shared_ptr<Shader> ShaderSkybox::Create()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

    (*shader).
        attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")

            .add_var("uniform", "mat4", "view")
            .add_var("uniform", "mat4", "projection")

            .add_var("out", "vec3", "TexCoords")

            .add_func("void", "main", "", R"_main_(
                TexCoords = aPos;
                vec4 pos = projection * view * vec4(aPos, 1.0);
                gl_Position = pos.xyww;
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("in", "vec3", "TexCoords")
            .add_var("uniform", "samplerCube", "skybox")
            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = texture(skybox, TexCoords);
            )_main_")
        )
    ;
	return shader;
}
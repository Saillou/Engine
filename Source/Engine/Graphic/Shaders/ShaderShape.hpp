#pragma once

#include "_ShaderHelper.hpp"

// -- Definition --
struct ShaderShape : public ShaderHelper {
	static inline std::shared_ptr<Shader> Create();
};

// -- Implementation --
inline std::shared_ptr<Shader> ShaderShape::Create()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

    (*shader)
        .attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("uniform", "mat4", "projection")
            .add_var("uniform", "mat4", "LocalModel")
            .add_var("out", "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                gl_Position = projection * LocalModel * vec4(aPos.x, aPos.y, 0.0, 1.0);
                float tx = aPos.x > 0 ? 1.0 : 0.0;
                float ty = aPos.y > 0 ? 1.0 : 0.0;
                TexCoords = vec2(tx, ty);
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("in", "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "quadTexture")
            .add_var("uniform", "vec4", "background_color")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                vec2 tex_size = textureSize(quadTexture, 0);
                vec4 bk_color = tex_size.x * tex_size.y > 1 ? texture(quadTexture, TexCoords) : background_color;
                FragColor = bk_color;
            )_main_")
        )
    ;

	return shader;
}
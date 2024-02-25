#pragma once

#include "_ShaderHelper.hpp"

// -- Definition --
struct ShaderText : public ShaderHelper {
	static inline std::shared_ptr<Shader> Create();
};

// -- Implementation --
inline std::shared_ptr<Shader> ShaderText::Create()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

	(*shader)
        .attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec4", "vertex")
            .add_var("uniform", "mat4", "projection")
            .add_var("out",     "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                TexCoords   = vertex.zw;
                gl_Position = projection * vec4(vertex.xy, 0.01f, 1.0);
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("out", "vec4", "FragColor")
            .add_var("in",  "vec2", "TexCoords")
            .add_var("uniform", "sampler2D", "text")
            .add_var("uniform", "vec4", "textColor")
            .add_func("void", "main", "", R"_main_(
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                FragColor    = vec4(textColor) * sampled;
            )_main_")
        )
    ;

	return shader;
}
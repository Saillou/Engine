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
            .add_var("layout (location = 0) in", "vec2", "aPos")
            .add_var("uniform", "mat4", "Projection")
            .add_func("void", "main", "", R"_main_(
                gl_Position = Projection * vec4(aPos.x, aPos.y, 0.0, 1.0);
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("uniform", "vec4", "Color")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                FragColor = Color;
            )_main_")
        )
    ;

	return shader;
}
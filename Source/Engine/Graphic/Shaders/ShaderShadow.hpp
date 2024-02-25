#pragma once

#include "_ShaderHelper.hpp"

// -- Definition --
struct ShaderShadow : public ShaderHelper {
	static inline std::shared_ptr<Shader> Create();
};

// -- Implementation --
inline std::shared_ptr<Shader> ShaderShadow::Create()
{
    std::shared_ptr<Shader> shader = std::make_shared<Shader>();

    (*shader)
        .attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")
            .add_var("layout (location = 3) in", "vec4", "aColor")
            .add_var("layout (location = 4) in", "mat4", "aModel")

            .add_var("uniform", "mat4", "LocalModel")
            .add_func("void", "main", "", R"_main_(
                gl_Position = aModel * LocalModel * vec4(aPos, 1.0);
            )_main_")
        )
        .attachSource(Shader::Geometry, ShaderSource{}
            .add_var("layout", "(triangles)", "in")
            .add_var("uniform", "mat4", "shadowMatrices[6]")
            .add_var("layout", "(triangle_strip, max_vertices = 18)", "out")
            .add_var("out", "vec4", "FragPos")
            .add_func("void", "main", "", R"_main_(
                for(int face = 0; face < 6; face++) {
                    gl_Layer = face;                    // built-in variable: face rendered
                    for(int i = 0; i < 3; ++i) {
                        FragPos = gl_in[i].gl_Position;
                        gl_Position = shadowMatrices[face] * FragPos;
                        EmitVertex();
                    }
                    EndPrimitive();
                }
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("in", "vec4", "FragPos")
            .add_var("uniform", "vec3", "lightPos")
            .add_var("uniform", "float", "far_plane")
            .add_func("void", "main", "", R"_main_(
                // map to [0;1] range by dividing by far_plane as modified depth
                gl_FragDepth = length(FragPos.xyz - lightPos) / far_plane;
            )_main_")
        )
    ;
    return shader;
}

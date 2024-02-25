#pragma once

#include "_ShaderHelper.hpp"

// -- Definition --
struct ShaderGeometry : public ShaderHelper {
	static inline std::shared_ptr<Shader> Create();
};

// -- Implementation --
inline std::shared_ptr<Shader> ShaderGeometry::Create()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

    (*shader)
        .attachSource(Shader::Vertex, 
            _init_vertex()

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos  = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Color    = aColor;
                gl_Position     = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
        )
        .attachSource(Shader::Geometry,
            _init_geometry()

            .add_var("layout", "(triangles)", "in")
            .add_var("layout", "(line_strip, max_vertices = 6)", "out")
            .add_var("out", "vec4", "Color")
            .add_var("uniform", "highp int", "n_lines")

            // line order: 0-1, 1-2, 2-0
            .add_func("void", "main", "", R"_main_(
                for(int i = 0; i < n_lines; i++) 
                {
                    Color = gs_in[i].Color;

                    gl_Position = gl_in[(i+0)%3].gl_Position; EmitVertex();
                    gl_Position = gl_in[(i+1)%3].gl_Position; EmitVertex(); 

                    EndPrimitive();
                }
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("in", "vec4", "Color")
            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                FragColor = Color;
            )_main_")
        );

	return shader;
}

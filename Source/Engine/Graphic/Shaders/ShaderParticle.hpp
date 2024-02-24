#pragma once

#include "_ShaderHelper.hpp"

// -- Definition --
struct ShaderParticle : public ShaderHelper {
	static inline std::shared_ptr<Shader> Create();
};

// -- Implementation --
inline std::shared_ptr<Shader> ShaderParticle::Create()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();

	(*shader)
        .attachSource(Shader::Vertex, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("layout (location = 1) in", "vec3", "aNormal")
            .add_var("layout (location = 2) in", "vec2", "aTexCoords")
            .add_var("layout (location = 3) in", "vec4", "aColor")
            .add_var("layout (location = 4) in", "mat4", "aModel")

            .add_var("uniform", "mat4", "Projection")
            .add_var("uniform", "mat4", "View")
            .add_var("uniform", "mat4", "LocalModel")

            .add_var("out", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec4 Color;
            } vs_out)_struct_")

            .add_func("void", "main", "", R"_main_(
                vs_out.FragPos = vec3(aModel * LocalModel * vec4(aPos, 1.0));
                vs_out.Color   = aColor;
                gl_Position    = Projection * View * vec4(vs_out.FragPos, 1.0);
            )_main_")
        )
        .attachSource(Shader::Geometry, ShaderSource{}
            .add_var("layout", "(triangles)", "in")
            .add_var("layout", "(triangle_strip, max_vertices = 3)", "out")

            .add_var("in", "VS_OUT", R"_struct_({
                vec3 FragPos;
                vec4 Color;
            } gs_in[])_struct_")

            .add_var("out", "vec3",  "Center")
            .add_var("out", "vec3",  "FragPos")
            .add_var("out", "vec4",  "Color")
            .add_var("out", "float", "Radius")

            .add_func("void", "main", "", R"_main_(
                // Compute circle dimensions (by our current quad definition: gs_in[1] is the right angle apex)
                Center = 0.5 * (gs_in[0].FragPos + gs_in[2].FragPos);
                Radius = 0.5 * distance(gs_in[0].FragPos, gs_in[1].FragPos);

                // Emit triangle
                for(int i = 0; i < 3; i++) {
                    FragPos = gs_in[i].FragPos;
                    Color   = gs_in[i].Color;

                    gl_Position = gl_in[i].gl_Position;
                    EmitVertex();
                }
    
                EndPrimitive();
            )_main_")
        )
        .attachSource(Shader::Fragment, ShaderSource{}
            .add_var("in", "vec3",  "Center")
            .add_var("in", "vec3",  "FragPos")
            .add_var("in", "vec4",  "Color")
            .add_var("in", "float", "Radius")

            .add_var("out", "vec4", "FragColor")

            .add_func("void", "main", "", R"_main_(
                float dist = distance(Center, FragPos);
                float smooth_dist = Radius * 4.0 / 100.0;
                float smooth_fact = 1.0;

                // None
                if(dist > Radius) {
                    discard;
                }

                // Smooth
                else if(dist > Radius - smooth_dist)
                    smooth_fact = (Radius - dist) / smooth_dist;

                // Solid
                else
                    smooth_fact = 1.0;
                
                FragColor = vec4(Color.rgb, Color.a * smooth_fact);
            )_main_")
        );

	return shader;
}
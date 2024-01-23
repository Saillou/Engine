#include "Ui.hpp"

Ui::Ui(Scene& scene) : 
    m_scene(scene),
    m_widget(scene)
{
    // Define events
    m_widget.on_draw() = [&]() { 
        draw(); 
    };

    // Setup root widget
    m_widget.opacity = 1.0f;

    shad_quad
        .attachSource(GL_VERTEX_SHADER, ShaderSource{}
            .add_var("layout (location = 0) in", "vec3", "aPos")
            .add_var("out", "vec2", "TexCoords")
            .add_func("void", "main", "", R"_main_(
                gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
                float tx = aPos.x > 0 ? 1.0 : 0.0;
                float ty = aPos.y > 0 ? 1.0 : 0.0;
                TexCoords = vec2(tx, ty);
            )_main_")
        )
        .attachSource(GL_FRAGMENT_SHADER, ShaderSource{}
            .add_var("in", "vec2", "TexCoords")
            .add_var("uniform", "vec4", "background_color")
            .add_var("out", "vec4", "FragColor")
            .add_func("void", "main", "", R"_main_(
                FragColor = background_color;
            )_main_")
        )
        .link();
}

void Ui::draw() {
    if (!visible) {
        _cleanup();
        return;
    }

    m_scene.clear();

    Quad q(0.4f, 0.45f, 0.2f, 0.1f);
    shad_quad.use().set("background_color", glm::vec4(0.7f, 0.4f, 0.2f, 1.0f));
    q.drawElements();

    m_scene.renderer().text("Start", m_scene.width()/2.0f, m_scene.height()/2.0f);
}


void Ui::_cleanup() {
    if(m_widget.opacity > 0)
        m_widget.opacity -= 0.03f;
}
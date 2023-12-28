#include "Quad.hpp"

// - Quad -
Quad::Quad(float x, float y, float w, float h)
{
    // x in [0, 1] -> [x, x+w] -> [-1, 1] (same for y)
    // transform: val_1 = val * 2.0f - 1.0f
    
    float x1 = x*2.0f - 1.0f;
    float y1 = y*2.0f - 1.0f;

    float x2 = (x+w)*2.0f - 1.0f;
    float y2 = (y+h)*2.0f - 1.0f;

    // Shape
    _createQuad(
        glm::vec3(x1, y2, 0),
        glm::vec3(x1, y1, 0),
        glm::vec3(x2, y1, 0),
        glm::vec3(x2, y2, 0)
    );
    _bindArray();

    // Cook
    addRecipe(CookType::Quad);
}

void Quad::draw() {
    get(CookType::Quad)->use();

    bind();
    glDrawElements(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);
    unbind();
}
#include "Material.hpp"

// Helpers
std::vector<glm::vec4> Material::ExtractColors(const std::vector<Material>& materials) 
{
    std::vector<glm::vec4> colors;
    colors.reserve(materials.size());

    for (const Material& material : materials) {
        colors.push_back(material.diffuse_color);
    }
    return colors;
}
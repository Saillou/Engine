#include "Cube.hpp"

#include <vector>
#include <memory>
#include <glm/glm.hpp>

using namespace glm;

std::unique_ptr<Mesh> Cube::CreateMesh() {
	std::unique_ptr<Mesh> cubeMesh = std::make_unique<Mesh>();

    const mat3 world(1.0f);

    // Space dimensions
    const vec3 u = world[0];
    const vec3 v = world[1];
    const vec3 w = world[2];

    // Vertices positions
    const vec3 pA = -u + v - w;
    const vec3 pB = +u + v - w;
    const vec3 pC = +u - v - w;
    const vec3 pD = -u - v - w;

    const vec3 pE = -u + v + w;
    const vec3 pF = +u + v + w;
    const vec3 pG = +u - v + w;
    const vec3 pH = -u - v + w;

    // Texture coordinates
    const vec2 tA = glm::vec2(0.0f, 0.0f);
    const vec2 tB = glm::vec2(1.0f, 0.0f);
    const vec2 tC = glm::vec2(0.0f, 1.0f);
    const vec2 tD = glm::vec2(1.0f, 1.0f);

    // Create triangles
    createQuad(*cubeMesh, pD, pC, pB, pA, -w, tA, tB, tC, tD);
    createQuad(*cubeMesh, pH, pG, pF, pE, +w, tA, tB, tC, tD);

    createQuad(*cubeMesh, pE, pA, pD, pH, -u, tA, tB, tC, tD);
    createQuad(*cubeMesh, pF, pB, pC, pG, +u, tA, tB, tC, tD);

    createQuad(*cubeMesh, pD, pC, pG, pH, -v, tA, tB, tC, tD);
    createQuad(*cubeMesh, pA, pB, pF, pE, +v, tA, tB, tC, tD);

    // Send to gpu
    setupGPU(*cubeMesh);

	return cubeMesh;
}

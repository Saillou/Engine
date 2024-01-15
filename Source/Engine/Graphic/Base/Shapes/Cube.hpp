#pragma once

#include <memory>
#include "../Model/Mesh.hpp"

struct Cube {
	static std::unique_ptr<Mesh> CreateMesh();

private:
	static int  _addPoint		(Mesh&, const glm::vec3& vec, const glm::vec3& norm, const glm::vec2& uv);
	static void _addAsTriangle	(Mesh&, unsigned int i0, unsigned int i1, unsigned int i2);
	static void _createQuad		(Mesh&, const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3,
								 const glm::vec3& n,
								 const glm::vec2& T0, const glm::vec2& T1, const glm::vec2& T2, const glm::vec2& T3);
};


#pragma once

#include <array>

#include "../../Model/Mesh.hpp"

struct PrimitiveHelper 
{
	// Types
	using Triangle = std::array<glm::vec3, 3>;

	// Vertices helper
	static unsigned int  addPoint(Mesh&, const glm::vec3& vec);
	static unsigned int  addPoint(Mesh&, const glm::vec3& vec, const glm::vec3& norm, const glm::vec2& uv);

	static void addAsTriangle	 (Mesh&, unsigned int i0, unsigned int i1, unsigned int i2);

	static void createTriangle	 (Mesh&, const Triangle& t);
	static void createQuad		 (Mesh&, const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3);
	static void createQuad		 (Mesh&, const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3,
		const glm::vec3& n,
		const glm::vec2& T0, const glm::vec2& T1, const glm::vec2& T2, const glm::vec2& T3);

	static void setIndices		 (Mesh&, const std::vector<unsigned int>&);
	static void setVertices		 (Mesh&, const std::vector<float>& vx, const std::vector<float>& nx, const std::vector<float>& ux);

	// For inhertied instances
	virtual ~PrimitiveHelper() = default;
};
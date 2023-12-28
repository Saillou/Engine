#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "../Utils/Array.hpp"
#include "../Utils/Buffer.hpp"

#include <vector>

class BaseShape
{
public:
	BaseShape();
	virtual ~BaseShape() = default;

	void createBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models);
	void updateBatch(const std::vector<glm::vec4>& colors, const std::vector<glm::mat4>& models);

	virtual void bind();
	virtual void unbind();

	int indicesLength() const;
	int verticesLength() const;
	int normalsLength() const;

protected:
	virtual void _bindArray();

	int _addPoint(float x, float y, float z);
	int _addPoint(float x, float y, float z, float nx, float ny, float nz);

	int _addPoint(const glm::vec3& vec);
	int _addPoint(const glm::vec3& vec, const glm::vec3& norm);

	void _addAsLine(unsigned int i0, unsigned int i1);
	void _addAsTriangle(unsigned int i0, unsigned int i1, unsigned int i2);
	void _addAsQuad(unsigned int i0, unsigned int i1, unsigned int i2, unsigned int i3);

	void _createQuad(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3);
	void _createQuad(const glm::vec3& P0, const glm::vec3& P1, const glm::vec3& P2, const glm::vec3& P3, const glm::vec3& n);

	Array m_vao;
	Buffer m_vbo_vertices;
	Buffer m_vbo_normals;
	Buffer m_ebo;

	Buffer m_colors;
	Buffer m_instances;

	unsigned int m_current_attrib_id;
	std::vector<unsigned int> m_indices;
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
};

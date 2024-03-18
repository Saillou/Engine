#include "ShapeMesh.hpp"

// Statics
std::unique_ptr<ShapeMesh> ShapeMesh::_s_mesh = nullptr;

void ShapeMesh::Create() 
{
	struct _ShapeMesh_: ShapeMesh {
		_ShapeMesh_() : ShapeMesh() { }
	};

	_s_mesh = std::make_unique<_ShapeMesh_>();
}

void ShapeMesh::Fill(const CanvasShape& shape)
{
	if (!_s_mesh)
		Create();

	_s_mesh->_update_buffer(shape.points);
	_s_mesh->_draw_triangles();
}

void ShapeMesh::Stroke(const CanvasShape& shape)
{
	if (!_s_mesh)
		Create();

	_s_mesh->_update_buffer(shape.points);
	_s_mesh->_draw_lines();
}

// Instance
ShapeMesh::ShapeMesh():
	m_vbo(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW)
{
	m_vao.bind();
	m_vbo.bindData(std::vector<Vertex>());

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
}

void ShapeMesh::_draw_triangles()
{
	m_vao.bind();
	glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)m_vbo.size() / sizeof(Vertex));
}

void ShapeMesh::_draw_lines() 
{
	m_vao.bind();
	glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)m_vbo.size() / sizeof(Vertex));
}

void ShapeMesh::_update_buffer(const CanvasShape::Points& points)
{
	std::vector<Vertex> vertices;
	for (const CanvasShape::Point& point : points)
	{
		vertices.push_back(Vertex{ point.x, point.y });
	}
	_s_mesh->m_vbo.bindData(vertices);
}
